/*
 * This file is part of PurePerspective.
 *
 * PurePerspective is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PurePerspective is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PurePerspective.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Pure.h"

//==============================================================================
PurePerspectiveAudioProcessor::PurePerspectiveAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
    processorStateParameters(*this, nullptr, "PARAMETERS", createParameters())
{
    processorStateParameters.state.addListener(this);
    muteLeft.store(processorStateParameters.getRawParameterValue(PARAM_MUTE_LEFT_ID)->load());
    muteRight.store(processorStateParameters.getRawParameterValue(PARAM_MUTE_RIGHT_ID)->load());
    mono.store(processorStateParameters.getRawParameterValue(PARAM_MONO_ID)->load());
    swap.store(processorStateParameters.getRawParameterValue(PARAM_SWAP_ID)->load());
}

PurePerspectiveAudioProcessor::~PurePerspectiveAudioProcessor()
{
}

//==============================================================================
const juce::String PurePerspectiveAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PurePerspectiveAudioProcessor::acceptsMidi() const
{
    return false;
}

bool PurePerspectiveAudioProcessor::producesMidi() const
{
    return false;
}

bool PurePerspectiveAudioProcessor::isMidiEffect() const
{
    return false;
}

double PurePerspectiveAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PurePerspectiveAudioProcessor::getNumPrograms()
{
    return 1;
}

int PurePerspectiveAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PurePerspectiveAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PurePerspectiveAudioProcessor::getProgramName (int index)
{
    return {};
}

void PurePerspectiveAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PurePerspectiveAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void PurePerspectiveAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PurePerspectiveAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
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

void PurePerspectiveAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    auto _mono = mono.load();
    auto _swap = swap.load();
    auto _muteLeft = muteLeft.load();
    auto _muteRight = muteRight.load();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; i++) {
        buffer.clear(i, 0, buffer.getNumSamples());
    }
    
    for (int channel = 0; channel < totalNumInputChannels; channel++) {
        const float* inputData = buffer.getReadPointer(channel);
        float* outputData = buffer.getWritePointer(channel);
    
        for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
            
            outputData[sample] = inputData[sample];
            
            if (_muteLeft && channel == 0) {
                outputData[sample] = 0;
            }
            
            if (_muteRight && channel == 1) {
                outputData[sample] = 0;
            }
        }
    }
    
    if (_mono && totalNumInputChannels == 2) {
        // add the right (1) to the left (0)
        buffer.addFrom(0, 0, buffer, 1, 0, buffer.getNumSamples());
        
        // copy the combined left (0) to the right (1)
        buffer.copyFrom(1, 0, buffer, 0, 0, buffer.getNumSamples());
    }
    
    if (_swap && totalNumInputChannels == 2) {
        
        auto* leftIn = buffer.getReadPointer(0);
        auto* rightIn = buffer.getReadPointer(1);
        
        auto* leftOut = buffer.getWritePointer(0);
        auto* rightOut = buffer.getWritePointer(1);
        
        for (auto sample = 0; sample < buffer.getNumSamples(); sample++) {
            float left = leftIn[sample];
            float right = rightIn[sample];
            
            leftOut[sample] = right;
            rightOut[sample] = left;
        }
    }
}

//==============================================================================
bool PurePerspectiveAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PurePerspectiveAudioProcessor::createEditor()
{
    return new PurePerspectiveAudioProcessorEditor (*this);
}

//==============================================================================
void PurePerspectiveAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PurePerspectiveAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

AudioProcessorValueTreeState::ParameterLayout PurePerspectiveAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> parameters;
    
    parameters.push_back(std::make_unique<AudioParameterBool>(PARAM_MONO_ID, PARAM_MONO_NAME, false));
    parameters.push_back(std::make_unique<AudioParameterBool>(PARAM_SWAP_ID, PARAM_SWAP_NAME, false));
    parameters.push_back(std::make_unique<AudioParameterBool>(PARAM_MUTE_LEFT_ID, PARAM_MUTE_LEFT_NAME, false));
    parameters.push_back(std::make_unique<AudioParameterBool>(PARAM_MUTE_RIGHT_ID, PARAM_MUTE_RIGHT_NAME, false));

    return { parameters.begin(), parameters.end() };
}

void PurePerspectiveAudioProcessor::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property) {
    mono.store(processorStateParameters.getRawParameterValue(PARAM_MONO_ID)->load());
    swap.store(processorStateParameters.getRawParameterValue(PARAM_SWAP_ID)->load());
    muteLeft.store(processorStateParameters.getRawParameterValue(PARAM_MUTE_LEFT_ID)->load());
    muteRight.store(processorStateParameters.getRawParameterValue(PARAM_MUTE_RIGHT_ID)->load());
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PurePerspectiveAudioProcessor();
}
