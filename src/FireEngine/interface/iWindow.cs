namespace FireEngine
{
    interface iWindow
    {
        bool visible
        {
            get;
        }
        bool inwindowlist
        {
            get;
        }
        bool candock
        {
            get;
        }
        string title
        {
            get;
        }
        void OnGUI();
        void Hide();
        void Show();

    }
}
