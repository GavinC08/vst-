/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayAudioProcessorEditor::DelayAudioProcessorEditor (DelayAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    
    auto& params = processor.getParameters();

    juce::AudioParameterFloat* dryWetParameter = (juce::AudioParameterFloat*)params.getUnchecked(0);
    //This is where the GUI can access the parameter values.
    
    mDryWetSlider.setBounds(0, 0, 100, 100);
    mDryWetSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mDryWetSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    mDryWetSlider.setRange(dryWetParameter->range.start, dryWetParameter->range.end);
    mDryWetSlider.setValue(*dryWetParameter);
    addAndMakeVisible(mDryWetSlider);
    mDryWetSlider.onDragStart = [dryWetParameter] {dryWetParameter->beginChangeGesture();};
    mDryWetSlider.onValueChange = [this, dryWetParameter] { *dryWetParameter = mDryWetSlider.getValue();};
    mDryWetSlider.onDragEnd = [dryWetParameter] {dryWetParameter->endChangeGesture();};
    //this is our drywet slider. This codes defines the:
    //size
    //style
    //text box
    //range
    //value
    //visibility
    
    juce::AudioParameterFloat* FeedbackParameter = (juce::AudioParameterFloat*)params.getUnchecked(1);
    //This is where the GUI can access the parameter values.
    mFeedbackSlider.setBounds(100, 0, 100, 100);
    mFeedbackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mFeedbackSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    mFeedbackSlider.setRange(FeedbackParameter->range.start, FeedbackParameter->range.end);
    mFeedbackSlider.setValue(*FeedbackParameter);
    addAndMakeVisible(mFeedbackSlider);
    mFeedbackSlider.onDragStart = [FeedbackParameter] {FeedbackParameter->beginChangeGesture();};
    mFeedbackSlider.onValueChange = [this, FeedbackParameter] { *FeedbackParameter = mFeedbackSlider.getValue();};
    mFeedbackSlider.onDragEnd = [FeedbackParameter] {FeedbackParameter->endChangeGesture();};
    //this is our feedback slider
    
    juce::AudioParameterFloat* DelayTimeParameter = (juce::AudioParameterFloat*)params.getUnchecked(2);
    //This is where the GUI can access the parameter values.
    mDelayTimeSlider.setBounds(200, 0, 100, 100);
    mDelayTimeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mDelayTimeSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    mDelayTimeSlider.setRange(DelayTimeParameter->range.start, DelayTimeParameter->range.end);
    mDelayTimeSlider.setValue(*DelayTimeParameter);
    addAndMakeVisible(mDelayTimeSlider);
    mDelayTimeSlider.onDragStart = [DelayTimeParameter] {DelayTimeParameter->beginChangeGesture();};
    mDelayTimeSlider.onValueChange = [this, DelayTimeParameter] { *DelayTimeParameter = mDelayTimeSlider.getValue();};
    mDelayTimeSlider.onDragEnd = [DelayTimeParameter] {DelayTimeParameter->endChangeGesture();};
    //this is our Delay Time slider
    
    
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

DelayAudioProcessorEditor::~DelayAudioProcessorEditor()
{
}

//==============================================================================
void DelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    g.drawFittedText ("Hello Gavin!", getLocalBounds(), juce::Justification::centred, 1);
}

void DelayAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
