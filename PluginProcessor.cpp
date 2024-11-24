#include "PluginProcessor.h"

// Entry point for JUCE to create your AudioProcessor instance
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SamplePlaybackProcessor(); // Replace with your AudioProcessor class name
}

// Constructor
SamplePlaybackProcessor::SamplePlaybackProcessor()
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true))
#endif
{
    formatManager.registerBasicFormats();
    reverbMix = new juce::AudioParameterFloat("reverbMix", "Reverb Mix", 0.0f, 1.0f, 0.2f);
    delayTime = new juce::AudioParameterFloat("delayTime", "Delay Time (ms)", 10.0f, 1000.0f, 300.0f);

    addParameter(reverbMix);
    addParameter(delayTime);

    // Add sampler voices and sound slots
    for (int i = 0; i < 64; ++i)
        sampler.addVoice(new juce::SamplerVoice());
}

SamplePlaybackProcessor::~SamplePlaybackProcessor()
{
    formatReader = nullptr;
}

void SamplePlaybackProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    sampler.setCurrentPlaybackSampleRate(sampleRate);

    // Reset delayLine
    delayLine.reset(); // Reset internal state
    delayLine.setMaximumDelayInSamples(static_cast<int>(sampleRate * 2)); // Set max delay to 2 seconds
}

void SamplePlaybackProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    // Clear unused output channels
    for (auto i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Process sampler
    sampler.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    // Convert AudioBuffer to AudioBlock for DSP processing
    juce::dsp::AudioBlock<float> audioBlock(buffer);

    // Apply delay
    auto delaySamples = delayTime->get() * getSampleRate() / 1000.0;
    delayLine.setDelay(delaySamples); // Set the current delay length in samples
    delayLine.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    // Apply reverb
    juce::dsp::Reverb::Parameters reverbParams;
    reverbParams.dryLevel = 1.0f - reverbMix->get();
    reverbParams.wetLevel = reverbMix->get();
    reverb.setParameters(reverbParams);
    reverb.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
}

void SamplePlaybackProcessor::loadSample(const juce::File& sampleFile)
{
    formatReader = formatManager.createReaderFor(sampleFile);

    if (formatReader != nullptr)
    {
        juce::BigInteger allNotes;
        allNotes.setRange(0, 128, true);
        sampler.clearSounds();
        sampler.addSound(new juce::SamplerSound(
            sampleFile.getFileName(),
            *formatReader,
            allNotes,
            60,  // root MIDI note
            0.1, // attack time
            0.1, // release time
            10.0 // maximum sample length
        ));
    }
}

// Implement the required pure virtual method 'getTailLengthSeconds'
double SamplePlaybackProcessor::getTailLengthSeconds() const
{
    return 0.0; // Update this value as needed for your plugin
}


const juce::String SamplePlaybackProcessor::getName() const
{
    return "Sample Playback Processor";
}


bool SamplePlaybackProcessor::acceptsMidi() const
{
    return true; // Return true if the plugin processes MIDI input
}

bool SamplePlaybackProcessor::producesMidi() const
{
    return false; // Return true if the plugin produces MIDI output
}

bool SamplePlaybackProcessor::isMidiEffect() const
{
    return false; // Return true if this plugin is a MIDI effect
}


int SamplePlaybackProcessor::getNumPrograms()
{
    return 1; // Return the number of programs (usually 1 for most plugins)
}

int SamplePlaybackProcessor::getCurrentProgram()
{
    return 0; // Return the current program index
}

void SamplePlaybackProcessor::setCurrentProgram(int index)
{
    // Handle program change (if needed)
}

const juce::String SamplePlaybackProcessor::getProgramName(int index)
{
    return "Default Program"; // Return the program name
}

void SamplePlaybackProcessor::changeProgramName(int index, const juce::String& newName)
{
    // Handle program name change (if needed)
}

void SamplePlaybackProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // Serialize plugin state to `destData` (if needed)
}

void SamplePlaybackProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // Restore plugin state from `data` (if needed)
}

