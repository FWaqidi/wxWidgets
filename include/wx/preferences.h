///////////////////////////////////////////////////////////////////////////////
// Name:        wx/preferences.h
// Purpose:     Declaration of wxPreferencesEditor class.
// Author:      Vaclav Slavik
// Created:     2013-02-19
// RCS-ID:      $Id$
// Copyright:   (c) 2013 Vaclav Slavik <vslavik@fastmail.fm>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_PREFERENCES_H_
#define _WX_PREFERENCES_H_

#include "wx/defs.h"
#include "wx/bitmap.h"
#include "wx/vector.h"

class WXDLLIMPEXP_FWD_CORE wxWindow;

class wxPreferencesEditorImpl;

#if defined(__WXOSX_COCOA__)
    // GetLargeIcon() is used
    #define wxHAS_PREF_EDITOR_ICONS
    // Changes should be applied immediately
    #define wxHAS_PREF_EDITOR_APPLY_IMMEDIATELY
#elif defined(__WXGTK__)
    // Changes should be applied immediately
    #define wxHAS_PREF_EDITOR_APPLY_IMMEDIATELY
#endif

// ----------------------------------------------------------------------------
// wxPreferencesEditor: Native preferences editing
// ----------------------------------------------------------------------------

// One page of a preferences window
class WXDLLIMPEXP_CORE wxPreferencesPage
{
public:
    wxPreferencesPage() {}
    virtual ~wxPreferencesPage() {}

    // Name of the page, used e.g. for tabs
    virtual wxString GetName() const = 0;

    // Return 32x32 icon used for the page. Currently only used on OS X, where
    // implementation is required; unused on other platforms. Because of this,
    // the method is only pure virtual on platforms that use it.
#ifdef wxHAS_PREF_EDITOR_ICONS
    virtual wxBitmap GetLargeIcon() const = 0;
#else
    virtual wxBitmap GetLargeIcon() const { return wxBitmap(); }
#endif

    // Create a window (usually a wxPanel) for this page. The caller takes
    // ownership of the returned window.
    virtual wxWindow *CreateWindow(wxWindow *parent) = 0;

    wxDECLARE_NO_COPY_CLASS(wxPreferencesPage);
};


// Helper for implementing some common pages (General, Advanced)
class WXDLLIMPEXP_CORE wxStockPreferencesPage : public wxPreferencesPage
{
public:
    enum Kind
    {
        Kind_General,
        Kind_Advanced
    };

    wxStockPreferencesPage(Kind kind) : m_kind(kind) {}
    Kind GetKind() const { return m_kind; }

    virtual wxString GetName() const;
#ifdef __WXOSX_COCOA__
    virtual wxBitmap GetLargeIcon() const;
#endif

private:
    Kind m_kind;
};


// Notice that this class does not inherit from wxWindow.
class WXDLLIMPEXP_CORE wxPreferencesEditor
{
public:
    // Ctor creates an empty editor, use AddPage() to add controls to it.
    wxPreferencesEditor();
    ~wxPreferencesEditor();

    // Add a new page to the editor. The editor takes ownership of the page
    // and won't delete it until it is destroyed itself.
    void AddPage(wxPreferencesPage *page);

    // Show the preferences dialog or bring it to the top if it's already
    // shown. Notice that this method may or may not block depending on the
    // platform, i.e. depending on whether the dialog is modal or not.
    void Show(wxWindow* parent);

    // Hide the currently shown dialog, if any. This doesn't do anything on the
    // platforms using modal preferences dialogs but should be called to
    // dismiss the dialog if the object whose preferences it is editing was
    // closed.
    void Dismiss();

    // Whether changes to values in the pages should be applied immediately
    // (OS X, GTK+) or only when the user clicks OK/Apply (Windows)
    static bool ShouldApplyChangesImmediately()
    {
#ifdef wxHAS_PREF_EDITOR_APPLY_IMMEDIATELY
        return true;
#else
        return false;
#endif
    }

private:
    wxPreferencesEditorImpl* m_impl;

    wxDECLARE_NO_COPY_CLASS(wxPreferencesEditor);
};

#endif // _WX_PREFERENCES_H_
