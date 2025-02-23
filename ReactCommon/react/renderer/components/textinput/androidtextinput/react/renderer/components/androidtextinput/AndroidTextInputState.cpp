/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "AndroidTextInputState.h"

#include <react/renderer/components/text/conversions.h>
#include <react/renderer/debug/debugStringConvertibleUtils.h>

#include <utility>

namespace facebook {
namespace react {

AndroidTextInputState::AndroidTextInputState(
    int64_t mostRecentEventCount,
    AttributedString attributedString,
    AttributedString reactTreeAttributedString,
    ParagraphAttributes paragraphAttributes,
    TextAttributes defaultTextAttributes,
    ShadowView defaultParentShadowView,
    float defaultThemePaddingStart,
    float defaultThemePaddingEnd,
    float defaultThemePaddingTop,
    float defaultThemePaddingBottom)
    : mostRecentEventCount(mostRecentEventCount),
      cachedAttributedStringId(0),
      attributedString(std::move(attributedString)),
      reactTreeAttributedString(std::move(reactTreeAttributedString)),
      paragraphAttributes(std::move(paragraphAttributes)),
      defaultTextAttributes(std::move(defaultTextAttributes)),
      defaultParentShadowView(std::move(defaultParentShadowView)),
      defaultThemePaddingStart(defaultThemePaddingStart),
      defaultThemePaddingEnd(defaultThemePaddingEnd),
      defaultThemePaddingTop(defaultThemePaddingTop),
      defaultThemePaddingBottom(defaultThemePaddingBottom) {}

AndroidTextInputState::AndroidTextInputState(
    AndroidTextInputState const &previousState,
    folly::dynamic const &data)
    : mostRecentEventCount(data.getDefault(
                                   "mostRecentEventCount",
                                   previousState.mostRecentEventCount)
                               .getInt()),
      cachedAttributedStringId(data.getDefault(
                                       "opaqueCacheId",
                                       previousState.cachedAttributedStringId)
                                   .getInt()),
      attributedString(previousState.attributedString),
      reactTreeAttributedString(previousState.reactTreeAttributedString),
      paragraphAttributes(previousState.paragraphAttributes),
      defaultTextAttributes(previousState.defaultTextAttributes),
      defaultParentShadowView(previousState.defaultParentShadowView),
      defaultThemePaddingStart(data.getDefault(
                                       "themePaddingStart",
                                       previousState.defaultThemePaddingStart)
                                   .getDouble()),
      defaultThemePaddingEnd(data.getDefault(
                                     "themePaddingEnd",
                                     previousState.defaultThemePaddingEnd)
                                 .getDouble()),
      defaultThemePaddingTop(data.getDefault(
                                     "themePaddingTop",
                                     previousState.defaultThemePaddingTop)
                                 .getDouble()),
      defaultThemePaddingBottom(data.getDefault(
                                        "themePaddingBottom",
                                        previousState.defaultThemePaddingBottom)
                                    .getDouble()){};

#ifdef ANDROID
folly::dynamic AndroidTextInputState::getDynamic() const {
  // Java doesn't need all fields, so we don't pass them all along.
  folly::dynamic newState = folly::dynamic::object();

  // If we have a `cachedAttributedStringId` we know that we're (1) not trying
  // to set a new string, so we don't need to pass it along; (2) setState was
  // called from Java to trigger a relayout with a `cachedAttributedStringId`,
  // so Java has all up-to-date information and we should pass an empty map
  // through.
  if (cachedAttributedStringId == 0) {
    newState["mostRecentEventCount"] = mostRecentEventCount;
    newState["attributedString"] = toDynamic(attributedString);
    newState["hash"] = newState["attributedString"]["hash"];
    newState["paragraphAttributes"] =
        toDynamic(paragraphAttributes); // TODO: can we memoize this in Java?
  }
  return newState;
}
#endif

} // namespace react
} // namespace facebook
