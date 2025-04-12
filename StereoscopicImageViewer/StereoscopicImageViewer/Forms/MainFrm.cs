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
            if (tscbSignalSource.SelectedIndex == (int)clsStereoImageManagerWrap.eSignalSources.COMPort)
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
            tbGlassesTimeOffset.Value = Settings.GlassesTimeOffset;
            lblGlassesTimeOffset.Text = Settings.GlassesTimeOffset.ToString();
            tbGlassesTimeOffset.Enabled = false;
            tbTransparentTimePercent.Value = Settings.TransparentTimePercent;
            lblTransparentTimePercent.Text = Settings.TransparentTimePercent.ToString();
            tbTransparentTimePercent.Enabled = false;
            tsslFrequencyLabel.Visible = false;
            tsslFrequency.Visible = false;
            //-----------------------------------
            //Apply these settings.
            SetVisible();
            SetAlwaysOnTop();
            SetRunAtStartup();
            //-----------------------------------
            LoadStereoImages();
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
            if (this.Visible)
            {
                mWasStartedWhenMinimized = mIsStarted;
                if (mIsStarted) PerformStop();
            }
            else
            {
                if (mWasStartedWhenMinimized)
                {
                    if (!mIsStarted) PerformStart();
                }
            }
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
        private bool mWasStartedWhenMinimized = false;
        private clsStereoImageManager mStereoImageManager = null;
        private clsFrequencyCounter mFrequencyCounter = null;
        private Task mTask = null;
        private bool mTaskQuit = false;
        private int mFrequencyInHz = 0;
        private int mLastError = (int)clsStereoImageManagerWrap.eStereoImageManagerErrors.NoError;
        private string mLeftImagePath = null;
        private string mRightImagePath = null;
        private bool mAlreadySent = true;
        private int mAlreadySentCounter = 0;
        #endregion

        #region Initialize
        private void Init()
        {
            timerFrequency.Start();
            timerErrors.Start();
            timerSendSettings.Start();
        }
        #endregion

        #region Menu Events
        private void visitWebSiteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("https://www.soft-collection.com");
        }
        private void gitHubToolStripMenuItem_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("https://github.com/Soft-Collection/StereoscopicImageViewer");
        }
        private void arduinoProjectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("https://github.com/Soft-Collection/StereoscopicImageViewer/blob/master/ArduinoProject/CableGlasses/CableGlasses.ino");
        }
        private void createImagesFromAnaglyphImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("https://github.com/Soft-Collection/StereoscopicImageViewer/blob/master/StereoImageSplitter/stereo_image_splitter.py");
        }
        private void stereoImagesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("https://github.com/Soft-Collection/StereoscopicImageViewer/tree/master/StereoImages");
        }
        #endregion

        #region Main Form Events
        private void tsbOpenFolder_Click(object sender, EventArgs e)
        {
            string temp = OpenFolder();
            Settings.FolderPath = (temp == string.Empty) ? Settings.FolderPath : temp;
            LoadStereoImages();
        }
        private void refreshToolStripMenuItem_Click(object sender, EventArgs e)
        {
            PerformStop();
            LoadStereoImages();
        }
        private void tscbFrequency_SelectedIndexChanged(object sender, EventArgs e)
        {
            Settings.Frequency = (clsStereoImageManagerWrap.eFrequencies)tscbFrequency.SelectedIndex;
        }
        private void tscbSignalSource_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (tscbSignalSource.SelectedIndex == 0)
            {
                Settings.SignalSource = clsStereoImageManagerWrap.eSignalSources.ScreenSensor;
                tscbComPort.Visible = false;
                tsbRefresh.Visible = false;
            }
            else if (tscbSignalSource.SelectedIndex == 1)
            {
                Settings.SignalSource = clsStereoImageManagerWrap.eSignalSources.COMPort;
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
                tsbRefresh.Visible = true;
            }
        }
        private void tscbComPort_SelectedIndexChanged(object sender, EventArgs e)
        {
            Settings.ComPort = (string)tscbComPort.SelectedItem;
        }
        private void tsbRefresh_Click(object sender, EventArgs e)
        {
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
        }
        private void tsbStartStop_Click(object sender, EventArgs e)
        {
            if (mIsStarted) PerformStop(); else PerformStart();
        }
        private void timerFrequency_Tick(object sender, EventArgs e)
        {
            tsslFrequency.Text = mFrequencyInHz.ToString() + "Hz";
        }
        private void timerErrors_Tick(object sender, EventArgs e)
        {
            int tempLastError = (int)clsStereoImageManagerWrap.eStereoImageManagerErrors.NoError;
            if (mLastError != (int)clsStereoImageManagerWrap.eStereoImageManagerErrors.NoError)
            {
                PerformStop();
                Interlocked.Exchange(ref tempLastError, mLastError);
                Interlocked.Exchange(ref mLastError, (int)clsStereoImageManagerWrap.eStereoImageManagerErrors.NoError);
            }
            if (tempLastError == (int)clsStereoImageManagerWrap.eStereoImageManagerErrors.DifferentLeftRightImageDimensions)
            {
                MessageBox.Show(
                    "Left and Right Images have different dimensions.", // Message
                    "Warning", // Title
                    MessageBoxButtons.OK, // OK button
                    MessageBoxIcon.Information // Icon
                );
            }
            tsbStartStop.Enabled = ((mLeftImagePath != null) && (mLeftImagePath != string.Empty) && (File.Exists(mLeftImagePath)) && (mRightImagePath != null) && (mRightImagePath != string.Empty) && (File.Exists(mRightImagePath)));
        }
        private void timerSendSettings_Tick(object sender, EventArgs e)
        {
            if (!mAlreadySent)
            {
                if (mAlreadySentCounter > 5)
                {
                    if (mStereoImageManager != null)
                    {
                        clsStereoImageManagerWrap.eStereoImageManagerErrors res1 = mStereoImageManager.SetGlassesTimeOffset(Settings.GlassesTimeOffset);
                        clsStereoImageManagerWrap.eStereoImageManagerErrors res2 = mStereoImageManager.SetTransparentTimePercent(Settings.TransparentTimePercent);
                    }
                    mAlreadySent = true;
                }
                mAlreadySentCounter++;
            }
        }
        private void lvStereoImages_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (lvStereoImages.SelectedItems.Count > 0)
            {
                if (mIsStarted) PerformStop();
                mLeftImagePath = Settings.FolderPath + "\\" + lvStereoImages.SelectedItems[0].Text + ".left.png";
                mRightImagePath = Settings.FolderPath + "\\" + lvStereoImages.SelectedItems[0].Text + ".right.png";
                if (!mIsStarted) PerformStart();
            }
        }
        private void tbGlassesTimeOffset_Scroll(object sender, EventArgs e)
        {
            lblGlassesTimeOffset.Text = tbGlassesTimeOffset.Value.ToString();
            Settings.GlassesTimeOffset = tbGlassesTimeOffset.Value;
            if (mStereoImageManager != null)
            {
                clsStereoImageManagerWrap.eStereoImageManagerErrors res = mStereoImageManager.SetGlassesTimeOffset(Settings.GlassesTimeOffset);
            }
        }
        private void tbTransparentTimePercent_Scroll(object sender, EventArgs e)
        {
            lblTransparentTimePercent.Text = tbTransparentTimePercent.Value.ToString();
            Settings.TransparentTimePercent = tbTransparentTimePercent.Value;
            if (mStereoImageManager != null)
            {
                clsStereoImageManagerWrap.eStereoImageManagerErrors res = mStereoImageManager.SetTransparentTimePercent(Settings.TransparentTimePercent);
            }
        }
        #endregion

        #region Methods
        private void PerformStart()
        {
            //To Be Started here.
            if (!mIsStarted)
            {
                mIsStarted = true;
                tsbStartStop.Text = "Stop";
                tsbStartStop.ToolTipText = "Stop";
                tsbStartStop.Image = global::StereoscopicImageViewer.Properties.Resources.stop;
                tscbFrequency.Enabled = false;
                tscbSignalSource.Enabled = false;
                tscbComPort.Enabled = false;
                tsbRefresh.Enabled = false;
                tsbOpenFolder.Enabled = false;
                tbGlassesTimeOffset.Enabled = true;
                tbTransparentTimePercent.Enabled = true;
                tsslFrequencyLabel.Visible = true;
                tsslFrequency.Visible = true;
                Start();
                mAlreadySent = false;
                mAlreadySentCounter = 0;
            }
        }
        private void PerformStop()
        {
            //To Be Stopped here.
            if (mIsStarted)
            {
                Stop();
                tsbStartStop.Text = "Start";
                tsbStartStop.ToolTipText = "Start";
                tsbStartStop.Image = global::StereoscopicImageViewer.Properties.Resources.play;
                tscbFrequency.Enabled = true;
                tscbSignalSource.Enabled = true;
                tscbComPort.Enabled = true;
                tsbRefresh.Enabled = true;
                tsbOpenFolder.Enabled = true;
                tbGlassesTimeOffset.Enabled = false;
                tbTransparentTimePercent.Enabled = false;
                tsslFrequencyLabel.Visible = false;
                tsslFrequency.Visible = false;
                pbVideoPanel.Refresh();
                mIsStarted = false;
            }
        }
        private void Start()
        {
            pbVideoPanel.BackColor = System.Drawing.Color.DarkGray;
            //------------------------------------------------------
            mStereoImageManager = new clsStereoImageManager(pbVideoPanel.Handle, Settings.Frequency, Settings.SignalSource, Settings.ComPort, mLeftImagePath, mRightImagePath);
            mFrequencyCounter = new clsFrequencyCounter();
            //------------------------------------------------------
            if (mTask == null)
            {
                mTaskQuit = false;
                mTask = new Task(() =>
                {
                    while (!mTaskQuit)
                    {
                        int frequencyInHz = 0;
                        clsStereoImageManagerWrap.eStereoImageManagerErrors res = mStereoImageManager.VideoRender();
                        if (res == clsStereoImageManagerWrap.eStereoImageManagerErrors.NoError)
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
            if (mStereoImageManager != null)
            {
                mStereoImageManager.Dispose();
                mStereoImageManager = null;
            }
            mFrequencyCounter = null;
            mStereoImageManager = null;
        }
        private string OpenFolder()
        {
            fbdOpenFolder.Description = "Open Stereo Images Folder";
            fbdOpenFolder.SelectedPath = Settings.FolderPath;
            DialogResult dialogResult = fbdOpenFolder.ShowDialog();
            if (dialogResult == DialogResult.OK)
            {
                return fbdOpenFolder.SelectedPath;
            }
            return string.Empty;
        }
        private void LoadStereoImages()
        {
            if (Settings.FolderPath == null) return;
            if (Settings.FolderPath == string.Empty) return;
            if (!Directory.Exists(Settings.FolderPath)) return;
            lvStereoImages.Items.Clear();
            string[] files = Directory.GetFiles(Settings.FolderPath);
            foreach (string file in files)
            {
                string fileNameWithExtension = Path.GetFileName(file);
                if (fileNameWithExtension.EndsWith(".left.png"))
                {
                    string fileName = fileNameWithExtension.Replace(".left.png", "");
                    if (File.Exists(Settings.FolderPath + '\\' + fileName + ".right.png"))
                    {
                        lvStereoImages.Items.Add(fileName);
                    }
                }
            }
            mLeftImagePath = null;
            mRightImagePath = null;
        }
        #endregion

        #endregion
    }
}