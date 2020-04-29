
internal void
BuildPanelMaximize(Application_Links * App)
{
    View_ID CompilationBuffer = open_build_footer_panel(App);
    view_set_split(App, CompilationBuffer,
                   PanelSplitKind_Ratio_Min, 0.75f);
}

internal void
BuildPanelMinimize(Application_Links * App)
{
    View_ID CompilationBuffer = open_build_footer_panel(App);
    
    Face_ID Face = get_face_id(App, 0);
    Face_Metrics Metrics = get_face_metrics(App, Face);
    f32 LineHeight = Metrics.line_height;
    
    view_set_split(App, CompilationBuffer,
                   PanelSplitKind_FixedPixels_Min, LineHeight * 8.f);
}

CUSTOM_COMMAND_SIG(OpenBuildPanelOrResize)
CUSTOM_DOC("Opens the build panel in the event that it's not there, and toggles sizing otherwise.")
{
    local_persist b32 IsMinimized = false;
    IsMinimized ? BuildPanelMaximize(app) : BuildPanelMinimize(app);
    IsMinimized = !IsMinimized;
}