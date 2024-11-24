#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class SamplePlaybackEditor : public juce::AudioProcessorEditor
{
public:
    SamplePlaybackEditor(SamplePlaybackProcessor&);
    ~SamplePlaybackEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    SamplePlaybackProcessor& processor;

    juce::TextButton loadButton{ "Load Sample" };
    juce::Slider reverbMixSlider;
    juce::Slider delayTimeSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SamplePlaybackEditor)
};
