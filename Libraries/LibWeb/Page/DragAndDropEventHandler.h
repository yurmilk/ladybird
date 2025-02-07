/*
 * Copyright (c) 2024, Tim Flynn <trflynn89@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <LibGC/Ptr.h>
#include <LibJS/Heap/Cell.h>
#include <LibWeb/Forward.h>
#include <LibWeb/HTML/DragDataStore.h>
#include <LibWeb/Page/EventResult.h>
#include <LibWeb/PixelUnits.h>

namespace Web {

class DragAndDropEventHandler {
public:
    void visit_edges(JS::Cell::Visitor& visitor) const;

    bool has_ongoing_drag_and_drop_operation() const { return !m_drag_data_store.is_null(); }

    EventResult handle_drag_start(JS::Realm&, CSSPixelPoint screen_position, CSSPixelPoint page_offset, CSSPixelPoint client_offset, CSSPixelPoint offset, unsigned button, unsigned buttons, unsigned modifiers, Vector<HTML::SelectedFile> files);
    EventResult handle_drag_move(JS::Realm&, GC::Ref<DOM::Document>, GC::Ref<DOM::Node>, CSSPixelPoint screen_position, CSSPixelPoint page_offset, CSSPixelPoint client_offset, CSSPixelPoint offset, unsigned button, unsigned buttons, unsigned modifiers);
    EventResult handle_drag_leave(JS::Realm&, CSSPixelPoint screen_position, CSSPixelPoint page_offset, CSSPixelPoint client_offset, CSSPixelPoint offset, unsigned button, unsigned buttons, unsigned modifiers);
    EventResult handle_drop(JS::Realm&, CSSPixelPoint screen_position, CSSPixelPoint page_offset, CSSPixelPoint client_offset, CSSPixelPoint offset, unsigned button, unsigned buttons, unsigned modifiers);

private:
    enum class Cancelled {
        No,
        Yes,
    };
    EventResult handle_drag_end(JS::Realm&, Cancelled, CSSPixelPoint screen_position, CSSPixelPoint page_offset, CSSPixelPoint client_offset, CSSPixelPoint offset, unsigned button, unsigned buttons, unsigned modifiers);

    GC::Ref<HTML::DragEvent> fire_a_drag_and_drop_event(
        JS::Realm&,
        GC::Ptr<DOM::EventTarget> target,
        FlyString const& name,
        CSSPixelPoint screen_position,
        CSSPixelPoint page_offset,
        CSSPixelPoint client_offset,
        CSSPixelPoint offset,
        unsigned button,
        unsigned buttons,
        unsigned modifiers,
        GC::Ptr<DOM::EventTarget> related_target = nullptr);

    bool allow_text_drop(GC::Ref<DOM::Node>) const;

    void reset();

    RefPtr<HTML::DragDataStore> m_drag_data_store;

    // https://html.spec.whatwg.org/multipage/dnd.html#source-node
    GC::Ptr<DOM::EventTarget> m_source_node;

    // https://html.spec.whatwg.org/multipage/dnd.html#immediate-user-selection
    GC::Ptr<DOM::Node> m_immediate_user_selection;

    // https://html.spec.whatwg.org/multipage/dnd.html#current-target-element
    GC::Ptr<DOM::Node> m_current_target_element;

    // https://html.spec.whatwg.org/multipage/dnd.html#current-drag-operation
    FlyString m_current_drag_operation;
};

}
