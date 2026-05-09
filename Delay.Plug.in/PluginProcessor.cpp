/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayPlug_inAudioProcessor::DelayPlug_inAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    mCircularBufferLeft = nullptr;
    mCircularBufferRight = nullptr;
    mCircularBufferWriteHead = 0;
    mCircularBufferLength = 0;
    mDelayTimeInSamples = 0;
    mCircularBufferReadHead = 0;
}



DelayPlug_inAudioProcessor::~DelayPlug_inAudioProcessor()
{
        if (mCircularBufferLeft != nullptr) {
            delete [] mCircularBufferLeft;
            mCircularBufferLeft = nullptr;
        }
        
        if (mCircularBufferRight != nullptr) {
            delete [] mCircularBufferRight;
            mCircularBufferRight = nullptr;
        }
}

//==============================================================================
const juce::String DelayPlug_inAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelayPlug_inAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelayPlug_inAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelayPlug_inAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelayPlug_inAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelayPlug_inAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DelayPlug_inAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelayPlug_inAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DelayPlug_inAudioProcessor::getProgramName (int index)
{
    return {};
}

void DelayPlug_inAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DelayPlug_inAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mCircularBufferWriteHead = 0;
    mCircularBufferLength = sampleRate * MAX_DELAY_TIME;
    mDelayTimeInSamples = sampleRate * 0.5;

    // Use this method as the place to do any pre-playback
        // initialisation that you need..
    if (mCircularBufferLeft == nullptr) {
            mCircularBufferLeft = new float [(int)(sampleRate * MAX_DELAY_TIME)](); // trailing parens initialize as zeros
        }
        
        if (mCircularBufferRight == nullptr) {
            mCircularBufferRight = new float [(int)(sampleRate * MAX_DELAY_TIME)](); // trailing parens initialize as zeros
        }
    }


void DelayPlug_inAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DelayPlug_inAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DelayPlug_inAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
            buffer.clear (i, 0, buffer.getNumSamples());
   
    float* leftChannel = buffer.getWritePointer(0);
    float* rightChannel = buffer.getWritePointer(1);

        for (int i = 0; i < buffer.getNumSamples(); i++) {
            mCircularBufferLeft[mCircularBufferWriteHead] = leftChannel[i];
            mCircularBufferRight[mCircularBufferWriteHead] = rightChannel[i];
            
            mCircularBufferReadHead = mCircularBufferWriteHead - mDelayTimeInSamples;
            
            mCircularBufferWriteHead++;
            
            if (mCircularBufferWriteHead >= mCircularBufferLength) {
                mCircularBufferWriteHead = 0;
            }
            
            mCircularBufferReadHead = mCircularBufferWriteHead - mDelayTimeInSamples;
                    if (mCircularBufferReadHead < 0) {
                        mCircularBufferReadHead += mCircularBufferLength;
                    }
        }
    
   
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool DelayPlug_inAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DelayPlug_inAudioProcessor::createEditor()
{
    return new DelayPlug_inAudioProcessorEditor (*this);
}

//==============================================================================
void DelayPlug_inAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DelayPlug_inAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayPlug_inAudioProcessor();
}
