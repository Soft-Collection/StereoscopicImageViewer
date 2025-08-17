using System;
using System.ComponentModel;
using System.Windows.Forms;

namespace StereoscopicImageViewer
{
    public partial class ImagerPanel : UserControl
    {
        #region Enums
        public enum eButtonsStates
        {
            Playing,
            Stopped
        }
        #endregion

        #region Variables
        private eButtonsStates mButtonsState = eButtonsStates.Stopped;
        #endregion

        #region Properties
        [Browsable(true)]
        [Category("Behavior")]
        [Description("")]
        public eButtonsStates ButtonsState
        {
            get
            {
                return mButtonsState;
            }
            set
            {
                eButtonsStates previousButtonsState = mButtonsState;
                mButtonsState = value;
                switch (mButtonsState)
                {
                    case eButtonsStates.Playing:
                        bPlayStop.BackgroundImage = global::StereoscopicImageViewer.Properties.Resources.astop;
                        OnPlay?.Invoke(this, EventArgs.Empty);
                        break;
                    case eButtonsStates.Stopped:
                        bPlayStop.BackgroundImage = global::StereoscopicImageViewer.Properties.Resources.aplay;
                        OnStop?.Invoke(this, EventArgs.Empty);
                        break;
                }
            }
        }
        #endregion

        #region Events
        [Browsable(true)]
        [Category("Behavior")]
        [Description("")]
        public event EventHandler OnPlay;
        [Browsable(true)]
        [Category("Behavior")]
        [Description("")]
        public event EventHandler OnStop;
        #endregion

        #region Constructor
        public ImagerPanel()
        {
            InitializeComponent();
        }
        #endregion

        #region Methods
        #endregion

        #region Event Methods
        private void bPlayStop_Click(object sender, EventArgs e)
        {
            if (ButtonsState == eButtonsStates.Stopped)
            {
                ButtonsState = eButtonsStates.Playing;
            }
            else if (ButtonsState == eButtonsStates.Playing)
            {
                ButtonsState = eButtonsStates.Stopped;
            }
        }
        private void bStop_Click(object sender, EventArgs e)
        {
            ButtonsState = eButtonsStates.Stopped;
        }
        private void bPlayPause_MouseEnter(object sender, EventArgs e)
        {
            ttControls.Show("Play / Stop", (IWin32Window)sender, 5000);
        }
        private void bStop_MouseEnter(object sender, EventArgs e)
        {
            ttControls.Show("Stop the video", (IWin32Window)sender, 5000);
        }
        #endregion
    }
}