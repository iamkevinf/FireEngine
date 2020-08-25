namespace FireEngine
{
    interface iWindow
    {
        bool visible
        {
            get;
        }
        bool isInWIndowList
        {
            get;
        }
        bool canDock
        {
            get;
        }
        string title
        {
            get;
        }
        void OnGUI();
        void OnTick();
        void Hide();
        void Show();

    }
}
