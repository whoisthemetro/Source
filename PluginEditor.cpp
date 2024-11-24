#include "PluginEditor.h"

SamplePlaybackEditor::SamplePlaybackEditor(SamplePlaybackProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    // Load button
    addAndMakeVisible(loadButton);
    loadButton.onClick = [this]() {
        juce::FileChooser chooser("Select a WAV file", juce::File::getCurrentWorkingDirectory(), "*.wav");

        chooser.launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
                            [this](const juce::FileChooser& fc) {
                                auto file = fc.getResult();
                                if (file.existsAsFile()) {
                                    processor.loadSample(file);
                                } else {
                                    juce::Logger::writeToLog("No file selected or file does not exist.");
                                }
                            });
    };

    // Reverb mix slider
    addAndMakeVisible(reverbMixSlider);
    reverbMixSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    reverbMixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    reverbMixSlider.setRange(0.0, 1.0, 0.01);
    reverbMixSlider.setValue(0.2);
    reverbMixSlider.onValueChange = [this]() {
        processor.getParameters()[0]->setValueNotifyingHost((float)reverbMixSlider.getValue());
    };

    // Delay time slider
    addAndMakeVisible(delayTimeSlider);
    delayTimeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    delayTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    delayTimeSlider.setRange(10.0, 1000.0, 1.0);
    delayTimeSlider.setValue(300.0);
    delayTimeSlider.onValueChange = [this]() {
        processor.getParameters()[1]->setValueNotifyingHost((float)delayTimeSlider.getValue());
    };

    setSize(400, 300);
}

SamplePlaybackEditor::~SamplePlaybackEditor() {}

void SamplePlaybackEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
    g.setColour(juce::Colours::white);
    g.setFont(18.0f);
    g.drawFittedText("Sample Playback Device", getLocalBounds(), juce::Justification::centred, 1);
}

void SamplePlaybackEditor::resized()
{
    auto area = getLocalBounds();
    loadButton.setBounds(area.removeFromTop(50).reduced(10));
    reverbMixSlider.setBounds(area.removeFromTop(50).reduced(10));
    delayTimeSlider.setBounds(area.removeFromTop(50).reduced(10));
}
