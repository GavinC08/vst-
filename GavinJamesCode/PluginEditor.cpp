/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GavinJamesAudioProcessorEditor::GavinJamesAudioProcessorEditor (GavinJamesAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
 
    auto& params = audioProcessor.getParameters();
    juce::AudioParameterFloat* gainParameter = (juce::AudioParameterFloat*)params.getUnchecked(0);
    
    mGainControlSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mGainControlSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(mGainControlSlider);

    mGainControlSlider.onDragStart = [gainParameter] {
            gainParameter->beginChangeGesture();
        };
        
        mGainControlSlider.onValueChange = [this, gainParameter] {
        *gainParameter = mGainControlSlider.getValue();
        };
        
        mGainControlSlider.onDragEnd = [gainParameter] {
            gainParameter->endChangeGesture();
        };

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

GavinJamesAudioProcessorEditor::~GavinJamesAudioProcessorEditor()
{
}

//==============================================================================
void GavinJamesAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void GavinJamesAudioProcessorEditor::resized()
{
    mGainControlSlider.setBounds(0, 0, 100, 100);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
