using System;
using System.IO;
using System.Diagnostics;
using System.IO.Ports;
using System.Security.Permissions;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace StereoscopicImageViewer
{
    public partial class MainFrm : Form
    {
        #region Main
        public MainFrm()
        {
            InitializeComponent();
            Initialize();
            Init();
        }
        #endregion

        #region Template
        #region Initialize
        //Initialize.
        private void Initialize()
        {
            try
            {
                Process currentProcess = Process.GetCurrentProcess();
                currentProcess.PriorityClass = ProcessPriorityClass.RealTime;
            }
            catch (Exception)
            {
            }
            this.Text = GetAssemblyInfo.AssemblyTitle;
            this.Icon = global::StereoscopicImageViewer.Properties.Resources.program_icon;
            trayNotifyIcon.Icon = global::StereoscopicImageViewer.Properties.Resources.program_icon;
            //Load Settings from Registry.
            this.Location = Settings.Location;
            this.Size = Settings.Size;
            this.Visible = Settings.Visible;
            this.TopMost = Settings.AlwaysOnTop;
            //-----------------------------------
            tscbFrequency.SelectedIndex = (int)Settings.Frequency;
            tscbSignalSource.SelectedIndex = (int)Settings.SignalSource;
            if (tscbSignalSource.SelectedIndex == (int)clsSIMWrap.eSignalSources.COMPort)
            {
                if (tscbComPort.Items.Contains(Settings.ComPort))
                {
                    tscbComPort.SelectedItem = Settings.ComPort;
                }
                else
                {
                    tscbComPort.SelectedIndex = 0;
                }
            }
            //-----------------------------------
            tsbStartStop.Enabled = false;
            //-----------------------------------
            //Apply these settings.
            SetVisible();
            SetAlwaysOnTop();
            SetRunAtStartup();
        }
        #endregion

        #region Menu Events
        //When clicked on Hide or Show.
        private void hideOrShowToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ChangeVisible();
        }
        //When clicked on Run At Startup.
        private void runAtStartupToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ChangeRunAtStartup();
        }
        //When clicked on Always On Top.
        private void alwaysOnTopToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ChangeAlwaysOnTop();
        }
        private void ExitProg()
        {
            Application.Exit();
        }
        //When clicked on About.
        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AboutBox about = new AboutBox();
            about.TopMost = true;
            about.ShowDialog();
        }
        //When clicked on Exit.
        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ExitProg();
        }
        private void trayNotifyIcon_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left) ChangeVisible();
        }
        #endregion

        #region Set Settings and Update Menu Items
        //Changes Visible state.
        private void ChangeVisible()
        {
            this.Visible = !this.Visible;
            SetVisible();
        }
        //Apply current Visible value.
        private void SetVisible()
        {
            Settings.Visible = this.Visible;
            if (this.Visible)
            {
                hideOrShowToolStripMenuItem.Text = "Hide";
                trayNotifyIcon.Text = "Hide " + GetAssemblyInfo.AssemblyProduct;

                this.Opacity = 1;
            }
            else
            {
                hideOrShowToolStripMenuItem.Text = "Show";
                trayNotifyIcon.Text = "Show " + GetAssemblyInfo.AssemblyProduct;
                this.Opacity = 0;

            }
        }
        //Changes Run At Startup state.
        private void ChangeRunAtStartup()
        {
            runAtStartupToolStripMenuItem.Checked = !runAtStartupToolStripMenuItem.Checked;
            Settings.RunAtStartup = runAtStartupToolStripMenuItem.Checked;
        }
        //Apply current Run At Startup value.
        private void SetRunAtStartup()
        {
            runAtStartupToolStripMenuItem.Checked = Settings.RunAtStartup;
        }
        //Changes Always On Top state.
        private void ChangeAlwaysOnTop()
        {
            this.TopMost = !this.TopMost;
            SetAlwaysOnTop();
        }
        //Apply current Always On Top value.
        private void SetAlwaysOnTop()
        {
            Settings.AlwaysOnTop = this.TopMost;
            alwaysOnTopToolStripMenuItem.Checked = this.TopMost;
        }
        #endregion

        #region Main Form Events
        private void MainFrm_Move(object sender, EventArgs e)
        {
            Settings.Location = this.Location;
        }
        private void MainFrm_Resize(object sender, EventArgs e)
        {
            Settings.Size = this.Size;
            ResizeWindow();
        }
        private void MainFrm_FormClosing(object sender, FormClosingEventArgs e)
        {
            mTaskQuit = true;
            if (mTask != null)
            {
                mTask.Wait();
                mTask.Dispose();
                mTask = null;
            }
            //------------------------
            ExitProg();
        }
        #endregion

        #region WinProc
        private const int WM_SYSCOMMAND = 0x0112;
        private const int SC_MINIMIZE = 0xF020;
        [SecurityPermission(SecurityAction.LinkDemand, Flags = SecurityPermissionFlag.UnmanagedCode)]
        protected override void WndProc(ref Message m)
        {
            switch (m.Msg)
            {
                case WM_SYSCOMMAND:
                    int command = m.WParam.ToInt32() & 0xfff0;
                    if (command == SC_MINIMIZE)
                    {
                        try
                        {
                            ChangeVisible();
                            return;
                        }
                        catch
                        {
                        }
                    }
                    break;
            }
            base.WndProc(ref m);
        }
        #endregion
        #endregion

        #region StereoscopicImageViewer

        #region Variables
        private bool mIsStarted = false;
        private clsStereoImageManager mStereoImageManager = null;
        private clsFrequencyCounter mFrequencyCounter = null;
        private Control mWindow = null;
        private Task mTask = null;
        private bool mTaskQuit = false;
        private int mFrequencyInHz = 0;
        private int mLastError = (int)clsSIMWrap.eStereoImageManagerErrors.NoError;
        #endregion

        #region Initialize
        private void Init()
        {
            timerFrequency.Start();
            timerErrors.Start();
        }
        #endregion

        #region Menu Events
        private void visitWebSiteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("http://www.soft-collection.com");
        }
        #endregion

        #region Main Form Events
        private void tsbOpenLeft_Click(object sender, EventArgs e)
        {
            string temp = OpenImageFile(true);
            Settings.LeftImagePath = (temp == null) ? Settings.LeftImagePath : temp;
        }
        private void tsbOpenRight_Click(object sender, EventArgs e)
        {
            string temp = OpenImageFile(true);
            Settings.RightImagePath = (temp == null) ? Settings.RightImagePath : temp;
        }
        private void tscbFrequency_SelectedIndexChanged(object sender, EventArgs e)
        {
            Settings.Frequency = (clsSIMWrap.eFrequencies)tscbFrequency.SelectedIndex;
        }
        private void tscbSignalSource_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (tscbSignalSource.SelectedIndex == 0)
            {
                Settings.SignalSource = clsSIMWrap.eSignalSources.ScreenSensor;
                tscbComPort.Visible = false;
            }
            else if (tscbSignalSource.SelectedIndex == 1)
            {
                Settings.SignalSource = clsSIMWrap.eSignalSources.COMPort;
                // Get a list of all available COM ports
                string[] ports = SerialPort.GetPortNames();
                tscbComPort.Items.Clear();
                foreach (string port in ports)
                {
                    tscbComPort.Items.Add(port);
                }
                if (tscbComPort.Items.Contains(Settings.ComPort))
                {
                    tscbComPort.SelectedItem = Settings.ComPort;
                }
                else
                {
                    tscbComPort.SelectedIndex = 0;
                }
                tscbComPort.Visible = true;
            }
        }
        private void tscbComPort_SelectedIndexChanged(object sender, EventArgs e)
        {
            Settings.ComPort = (string)tscbComPort.SelectedItem;
        }
        private void tsbStartStop_Click(object sender, EventArgs e)
        {
            try
            {
                if (mIsStarted)
                {
                    PerformStop();
                }
                else //(!IsStarted)
                {
                    PerformStart();
                }
            }
            catch { }
        }
        private void timerFrequency_Tick(object sender, EventArgs e)
        {
            tsslFrequency.Text = mFrequencyInHz.ToString() + "Hz";
        }
        private void timerErrors_Tick(object sender, EventArgs e)
        {
            int tempLastError = (int)clsSIMWrap.eStereoImageManagerErrors.NoError;
            if (mLastError != (int)clsSIMWrap.eStereoImageManagerErrors.NoError)
            {
                PerformStop();
                Interlocked.Exchange(ref tempLastError, mLastError);
                Interlocked.Exchange(ref mLastError, (int)clsSIMWrap.eStereoImageManagerErrors.NoError);
            }
            if (tempLastError == (int)clsSIMWrap.eStereoImageManagerErrors.DifferentLeftRightImageDimensions)
            {
                MessageBox.Show(
                    "Left and Right Images have different dimensions.", // Message
                    "Warning", // Title
                    MessageBoxButtons.OK, // OK button
                    MessageBoxIcon.Information // Icon
                );
            }
            tsbStartStop.Enabled = ((Settings.LeftImagePath != null) && (Settings.LeftImagePath != string.Empty) && (File.Exists(Settings.LeftImagePath)));
            tsbStartStop.Enabled = ((Settings.RightImagePath != null) && (Settings.RightImagePath != string.Empty) && (File.Exists(Settings.RightImagePath)));
        }
        #endregion

        #region Methods
        private void PerformStart()
        {
            //To Be Started here.
            Start();
            tsbStartStop.Text = "Stop";
            tsbStartStop.ToolTipText = "Stop";
            tsbStartStop.Image = global::StereoscopicImageViewer.Properties.Resources.stop;
            tscbFrequency.Enabled = false;
            tscbSignalSource.Enabled = false;
            tscbComPort.Enabled = false;
            tsbOpenLeft.Enabled = false;
            tsbOpenRight.Enabled = false;
            mIsStarted = true;
        }
        private void PerformStop()
        {
            //To Be Stopped here.
            Stop();
            tsbStartStop.Text = "Start";
            tsbStartStop.ToolTipText = "Start";
            tsbStartStop.Image = global::StereoscopicImageViewer.Properties.Resources.play;
            tscbFrequency.Enabled = true;
            tscbSignalSource.Enabled = true;
            tscbComPort.Enabled = true;
            tsbOpenLeft.Enabled = true;
            tsbOpenRight.Enabled = true;
            mIsStarted = false;
        }
        private void ResizeWindow()
        {
            int windowWidth = pbVideoPanel.ClientRectangle.Width;
            int windowHeight = pbVideoPanel.ClientRectangle.Height;
            if (mWindow != null)
            {
                mWindow.Size = new System.Drawing.Size(new System.Drawing.Point(windowWidth, windowHeight));
                mWindow.Location = new System.Drawing.Point(0, 0);
            }
        }
        private void Start()
        {
            mWindow = new Control();
            mWindow.BackColor = System.Drawing.Color.Blue;
            pbVideoPanel.Controls.Add(mWindow);
            //------------------------------------------------------
            mStereoImageManager = new clsStereoImageManager(mWindow.Handle, Settings.Frequency, Settings.SignalSource, Settings.ComPort, Settings.LeftImagePath, Settings.RightImagePath);
            mFrequencyCounter = new clsFrequencyCounter();
            //------------------------------------------------------
            ResizeWindow();
            //------------------------------------------------------
            if (mTask == null)
            {
                mTask = new Task(() =>
                {
                    mTaskQuit = false;
                    while (!mTaskQuit)
                    {
                        int frequencyInHz = 0;
                        clsSIMWrap.eStereoImageManagerErrors res = mStereoImageManager.VideoRender();
                        if (res == clsSIMWrap.eStereoImageManagerErrors.NoError)
                        {
                            frequencyInHz += mFrequencyCounter.GetFrequencyInHzValue();
                            mFrequencyCounter.Increment();
                            Interlocked.Exchange(ref mFrequencyInHz, frequencyInHz);
                        }
                        else
                        {
                            Interlocked.Exchange(ref mLastError, (int)res);
                            mTaskQuit = true;
                        }
                    }
                });
                mTask.Start();
            }
        }
        private void Stop()
        {
            mTaskQuit = true;
            if (mTask != null)
            {
                mTask.Wait();
                mTask.Dispose();
                mTask = null;
            }
            //------------------------------------------------------
            if (mWindow != null)
            {
                pbVideoPanel.Controls.Remove(mWindow);
                mWindow.Dispose();
                mWindow = null;
            }
            //------------------------------------------------------
            if (mStereoImageManager != null)
            {
                mStereoImageManager.Dispose();
                mStereoImageManager = null;
            }
            mWindow = null;
            mFrequencyCounter = null;
            mStereoImageManager = null;
        }

        private string OpenImageFile(bool left)
        {
            try
            {
                dlgOpenImage.Title = "Open " + ((left) ? "Left" : "Right") + " Image File";
                dlgOpenImage.Filter = "List Files(*.png)|*.png";
                dlgOpenImage.AddExtension = true;
                dlgOpenImage.CheckFileExists = false;
                dlgOpenImage.FileName = "";
                DialogResult dialogResult = dlgOpenImage.ShowDialog();
                if (dialogResult == DialogResult.OK)
                {
                    return dlgOpenImage.FileName;
                }
            }
            catch
            {
            }
            return null;
        }
        #endregion

        #endregion
    }
}