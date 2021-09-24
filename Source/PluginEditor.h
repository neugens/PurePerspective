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


#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Pure.h"

//==============================================================================
/**
*/
class PurePerspectiveAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    PurePerspectiveAudioProcessorEditor (PurePerspectiveAudioProcessor&);
    ~PurePerspectiveAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void updateToggleState(juce::Button* button, juce::String name);

    
private:
    PurePerspectiveAudioProcessor& audioProcessor;

    juce::ToggleButton mono { "Mono" };
    juce::ToggleButton muteLeft { "Mute Left" };
    juce::ToggleButton muteRight { "Mute Right" };
    juce::ToggleButton swapChannels { "Swap Channels" };

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> muteLeftChannelAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> muteRightChannelAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> swapChannelsAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> monoAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PurePerspectiveAudioProcessorEditor)
};
