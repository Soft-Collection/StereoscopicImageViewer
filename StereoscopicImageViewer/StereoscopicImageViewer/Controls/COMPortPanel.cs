using System;
using System.Windows.Forms;
using System.IO.Ports;
using System.ComponentModel;

namespace StereoscopicImageViewer
{
    public partial class COMPortPanel : UserControl
    {
        #region Classes
        public class COMPortEventArgs : EventArgs
        {
            public string COMPort { get; }
            public COMPortEventArgs(string comPort)
            {
                COMPort = comPort;
            }
        }
        #endregion

        #region Enums
        #endregion

        #region Variables
        private string mSelectedCOMPort = string.Empty;
        #endregion

        #region Properties
        [Browsable(true)]
        [Category("Behavior")]
        [Description("")]
        public string SelectedCOMPort
        {
            get
            {
                return mSelectedCOMPort;
            }
            set
            {
                mSelectedCOMPort = value;
                OnCOMPortSelected?.Invoke(this, new COMPortEventArgs(SelectedCOMPort));
            }
        }
        [Browsable(true)]
        [Category("Behavior")]
        [Description("")]
        public bool IsCOMPortSelected
        {
            get
            {
                return cbComPort.SelectedIndex >= 0;
            }
        }
        #endregion

        #region Events
        [Browsable(true)]
        [Category("Behavior")]
        [Description("")]
        public event EventHandler<COMPortEventArgs> OnCOMPortSelected;
        #endregion

        #region Constructor
        public COMPortPanel()
        {
            InitializeComponent();
        }
        #endregion

        #region Methods
        public void LoadPorts()
        {
            // Get a list of all available COM ports
            string[] ports = SerialPort.GetPortNames();
            cbComPort.Items.Clear();
            foreach (string port in ports)
            {
                cbComPort.Items.Add(port);
            }
            if (cbComPort.Items.Contains(SelectedCOMPort))
            {
                cbComPort.SelectedItem = SelectedCOMPort;
            }
            else
            {
                cbComPort.SelectedIndex = -1;
            }
        }
        #endregion

        #region Event Methods
        private void bRefresh_Click(object sender, EventArgs e)
        {
            LoadPorts();
        }
        private void cbComPort_SelectedIndexChanged(object sender, EventArgs e)
        {
            SelectedCOMPort = (string)cbComPort.SelectedItem;
        }
        private void cbComPort_MouseEnter(object sender, EventArgs e)
        {
            ttControls.Show("Select COM Port of Arduino glasses controller", (IWin32Window)sender, 5000);
        }
        private void bRefresh_MouseEnter(object sender, EventArgs e)
        {
            ttControls.Show("Refresh COM Ports", (IWin32Window)sender, 5000);
        }
        #endregion
    }
}
