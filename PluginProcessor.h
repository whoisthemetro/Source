#pragma once

#include <JuceHeader.h>

class SamplePlaybackProcessor : public juce::AudioProcessor
{
public:
    SamplePlaybackProcessor();
    ~SamplePlaybackProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    double getTailLengthSeconds() const override; // Ensure 'const' matches the implementation

    void loadSample(const juce::File& sampleFile);

private:
    juce::Synthesiser sampler;
    juce::AudioFormatManager formatManager;
    juce::AudioFormatReader* formatReader = nullptr;

    juce::dsp::Reverb reverb;
    juce::dsp::DelayLine<float> delayLine;

    juce::AudioParameterFloat* reverbMix;
    juce::AudioParameterFloat* delayTime;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SamplePlaybackProcessor)
    
    const juce::String getName() const override;
    

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
   
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
};
