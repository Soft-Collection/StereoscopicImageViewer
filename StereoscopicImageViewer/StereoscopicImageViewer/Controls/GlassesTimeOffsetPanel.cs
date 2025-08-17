using System;
using System.ComponentModel;
using System.Windows.Forms;

namespace StereoscopicImageViewer
{
    public partial class GlassesTimeOffsetPanel : UserControl
    {
        #region Classes
        public class TimeOffsetEventArgs : EventArgs
        {
            public int TimeOffset { get; }
            public TimeOffsetEventArgs(int timeOffset)
            {
                TimeOffset = timeOffset;
            }
        }
        #endregion

        #region Enums
        #endregion

        #region Variables
        private int mTimeOffset = 0;
        #endregion

        #region Properties
        [Browsable(true)]
        [Category("Behavior")]
        [Description("")]
        public int TimeOffset
        {
            get
            {
                return mTimeOffset;
            }
            set
            {
                mTimeOffset = value;
                tbGlassesTimeOffset.Value = mTimeOffset;
                lblGlassesTimeOffset.Text = mTimeOffset.ToString();
                OnTimeOffset?.Invoke(this, new TimeOffsetEventArgs(TimeOffset));
            }
        }
        #endregion

        #region Events
        [Browsable(true)]
        [Category("Behavior")]
        [Description("")]
        public event EventHandler<TimeOffsetEventArgs> OnTimeOffset;
        #endregion

        #region Constructor
        public GlassesTimeOffsetPanel()
        {
            InitializeComponent();
        }
        #endregion

        #region Methods
        #endregion

        #region Event Methods
        private void tbGlassesTimeOffset_Scroll(object sender, EventArgs e)
        {
            TimeOffset = tbGlassesTimeOffset.Value;
        }
        private void tbGlassesTimeOffset_MouseEnter(object sender, EventArgs e)
        {
            ttControls.Show("Fine correction for the stereo effect", (IWin32Window)sender, 5000);
        }
        #endregion
    }
}