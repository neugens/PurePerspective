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
PurePerspectiveAudioProcessorEditor::PurePerspectiveAudioProcessorEditor (PurePerspectiveAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize(WIDTH, HEIGHT);
    
    monoAttachment = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.getProcessorStateParameters(),
                                                                                      PARAM_MONO_ID,
                                                                                      mono);
    mono.setEnabled(true);
    mono.setClickingTogglesState(true);
    mono.onClick = [this] { updateToggleState(&mono, PARAM_MONO_NAME); };
    
    swapChannelsAttachment = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.getProcessorStateParameters(),
                                                                                              PARAM_SWAP_ID,
                                                                                              swapChannels);
    swapChannels.setEnabled(true);
    swapChannels.setClickingTogglesState(true);
    swapChannels.onClick = [this] { updateToggleState(&swapChannels, PARAM_SWAP_NAME); };
    
    muteLeftChannelAttachment = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.getProcessorStateParameters(),
                                                                                                 PARAM_MUTE_LEFT_ID,
                                                                                                 muteLeft);
    muteLeft.setEnabled(true);
    muteLeft.setClickingTogglesState(true);
    muteLeft.onClick = [this] { updateToggleState(&muteLeft, PARAM_MUTE_LEFT_NAME); };
    
    muteRightChannelAttachment = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.getProcessorStateParameters(),
                                                                                                  PARAM_MUTE_RIGHT_ID,
                                                                                                  muteRight);
                                                                                                  
    muteRight.setEnabled(true);
    muteRight.setClickingTogglesState(true);
    muteRight.onClick = [this] { updateToggleState(&muteRight, PARAM_MUTE_RIGHT_NAME); };
    
    addAndMakeVisible(muteLeft);
    addAndMakeVisible(muteRight);
    addAndMakeVisible(mono);
    addAndMakeVisible(swapChannels);
}

PurePerspectiveAudioProcessorEditor::~PurePerspectiveAudioProcessorEditor()
{
}

void PurePerspectiveAudioProcessorEditor::updateToggleState(juce::Button* button, juce::String name)
{
//    auto state = button->getToggleState();
//    juce::String stateString = state ? "ON" : "OFF";    
//    juce::Logger::outputDebugString(name + " Button changed to " + stateString);
}


//==============================================================================
void PurePerspectiveAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    
    muteLeft.setBounds(BORDER_OFFSET, BORDER_OFFSET, BUTTON_W, BUTTON_H);
    muteRight.setBounds(BORDER_OFFSET + BUTTON_W + 1, BORDER_OFFSET, BUTTON_W, BUTTON_H);
    
    mono.setBounds(BORDER_OFFSET, BORDER_OFFSET + BUTTON_H + 1, BUTTON_W, BUTTON_H);
    swapChannels.setBounds(BORDER_OFFSET + BUTTON_W + 1, BORDER_OFFSET + BUTTON_H + 1, BUTTON_W, BUTTON_H);
}

void PurePerspectiveAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
