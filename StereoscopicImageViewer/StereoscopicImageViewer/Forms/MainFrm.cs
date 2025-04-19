using System;
using System.IO;
using System.Diagnostics;
using System.IO.Ports;
using System.Security.Permissions;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Collections.Generic;

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
            //-----------------------------------
            pbVideoPanel.BackColor = System.Drawing.Color.DarkGray;
            //------------------------------------------------------
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
            //-----------------------------------
            if (mStereoImageManager == null)
            {
                mStereoImageManager = new clsStereoImageManager(pbVideoPanel.Handle);
            }
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
            PerformStop();
            if (mStereoImageManager != null)
            {
                mStereoImageManager.Dispose();
                mStereoImageManager = null;
            }
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
                bool isStarted = false;
                if (mStereoImageManager != null)
                {
                    isStarted = mStereoImageManager.IsStarted();
                }
                mWasStartedWhenMinimized = isStarted;
                if (isStarted) PerformStop();
            }
            else
            {
                if (mWasStartedWhenMinimized)
                {
                    bool isStarted = false;
                    if (mStereoImageManager != null)
                    {
                        isStarted = mStereoImageManager.IsStarted();
                    }
                    if (!isStarted) PerformStart();
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
        private bool mWasStartedWhenMinimized = false;
        private clsStereoImageManager mStereoImageManager = null;
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
        private void openSelectedFolderInExplorerToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (Settings.FolderPath != string.Empty)
            {
                if (Directory.Exists(Settings.FolderPath))
                {
                    Process.Start("explorer.exe", Settings.FolderPath);
                }
                else
                {
                    MessageBox.Show(
                                       "Folder not found!", // Message
                                       "Warning", // Title
                                       MessageBoxButtons.OK, // OK button
                                       MessageBoxIcon.Information // Icon
                                   );
                }
            }
            else
            {
                MessageBox.Show(
                                   "Folder not selected!", // Message
                                   "Warning", // Title
                                    MessageBoxButtons.OK, // OK button
                                    MessageBoxIcon.Information // Icon
                               );
            }
        }
        private void refreshToolStripMenuItem_Click(object sender, EventArgs e)
        {
            PerformStop();
            LoadStereoImages();
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
            bool isStarted = false;
            if (mStereoImageManager != null)
            {
                isStarted = mStereoImageManager.IsStarted();
                if (isStarted) PerformStop(); else PerformStart();
            }
        }
        private void timerFrequency_Tick(object sender, EventArgs e)
        {
            int frequencyInHz = 0;
            if (mStereoImageManager != null)
            {
                frequencyInHz = mStereoImageManager.GetFrequency();
            }
            tsslFrequency.Text = frequencyInHz.ToString() + "Hz";
        }
        private void timerErrors_Tick(object sender, EventArgs e)
        {
            tsbStartStop.Enabled = ((mLeftImagePath != null) && 
                                    (mLeftImagePath != string.Empty) && 
                                    (File.Exists(mLeftImagePath)) && 
                                    (mRightImagePath != null) && 
                                    (mRightImagePath != string.Empty) && 
                                    (File.Exists(mRightImagePath)));
        }
        private void timerSendSettings_Tick(object sender, EventArgs e)
        {
            if (!mAlreadySent)
            {
                if (mAlreadySentCounter > 5)
                {
                    if (mStereoImageManager != null)
                    {
                        mStereoImageManager.SetGlassesTimeOffset(Settings.GlassesTimeOffset);
                        mStereoImageManager.SetTransparentTimePercent(Settings.TransparentTimePercent);
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
                mLeftImagePath = Settings.FolderPath + "\\" + ((Tuple<string, string>)lvStereoImages.SelectedItems[0].Tag).Item1;
                mRightImagePath = Settings.FolderPath + "\\" + ((Tuple<string, string>)lvStereoImages.SelectedItems[0].Tag).Item2;
                if (mStereoImageManager != null)
                {
                    mStereoImageManager.DrawImage(mLeftImagePath, mRightImagePath);
                }
            }
        }
        private void tbGlassesTimeOffset_Scroll(object sender, EventArgs e)
        {
            lblGlassesTimeOffset.Text = tbGlassesTimeOffset.Value.ToString();
            Settings.GlassesTimeOffset = tbGlassesTimeOffset.Value;
            if (mStereoImageManager != null)
            {
                mStereoImageManager.SetGlassesTimeOffset(Settings.GlassesTimeOffset);
            }
        }
        private void tbTransparentTimePercent_Scroll(object sender, EventArgs e)
        {
            lblTransparentTimePercent.Text = tbTransparentTimePercent.Value.ToString();
            Settings.TransparentTimePercent = tbTransparentTimePercent.Value;
            if (mStereoImageManager != null)
            {
                mStereoImageManager.SetTransparentTimePercent(Settings.TransparentTimePercent);
            }
        }
        #endregion

        #region Methods
        private void PerformStart()
        {
            bool isStarted = false;
            if (mStereoImageManager != null)
            {
                isStarted = mStereoImageManager.IsStarted();
                //To Be Started here.
                if (!isStarted)
                {
                    tsbStartStop.Text = "Stop";
                    tsbStartStop.ToolTipText = "Stop";
                    tsbStartStop.Image = global::StereoscopicImageViewer.Properties.Resources.stop;
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
        }
        private void PerformStop()
        {
            bool isStarted = false;
            if (mStereoImageManager != null)
            {
                isStarted = mStereoImageManager.IsStarted();
                //To Be Stopped here.
                if (isStarted)
                {
                    Stop();
                    tsbStartStop.Text = "Start";
                    tsbStartStop.ToolTipText = "Start";
                    tsbStartStop.Image = global::StereoscopicImageViewer.Properties.Resources.play;
                    tscbComPort.Enabled = true;
                    tsbRefresh.Enabled = true;
                    tsbOpenFolder.Enabled = true;
                    tbGlassesTimeOffset.Enabled = false;
                    tbTransparentTimePercent.Enabled = false;
                    tsslFrequencyLabel.Visible = false;
                    tsslFrequency.Visible = false;
                    pbVideoPanel.Refresh();
                }
            }
        }
        private void Start()
        {
            if (mStereoImageManager != null)
            {
                mStereoImageManager.SetCOMPort(Settings.ComPort);
                mStereoImageManager.Start();
            }
        }
        private void Stop()
        {
            if (mStereoImageManager != null)
            {
                mStereoImageManager.Stop();
            }
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
            if (!Directory.Exists(Settings.FolderPath))
            {
                Settings.FolderPath = string.Empty;
                return;
            }
            lvStereoImages.Items.Clear();
            string[] files = Directory.GetFiles(Settings.FolderPath);
            foreach (string file in files)
            {
                List<string> supportedExtensions = new List<string> { ".PNG",".png",".JPEG",".jpeg",".JPG",".jpg",".BMP",".bmp",".TIFF",".tiff",".GIF",".gif" };
                string fileNameWithExtension = Path.GetFileName(file);
                string fileExtension = Path.GetExtension(file);
                if (supportedExtensions.Contains(fileExtension))
                {
                    if (fileNameWithExtension.Contains(".left"))
                    {
                        string fileName = fileNameWithExtension.Replace(".left" + fileExtension, "");
                        if (File.Exists(Settings.FolderPath + '\\' + fileName + ".right" + fileExtension))
                        {
                            ListViewItem lvi = new ListViewItem();
                            lvi.Text = fileName;
                            lvi.Tag = new Tuple<string, string>(fileName + ".left" + fileExtension, fileName + ".right" + fileExtension);
                            lvStereoImages.Items.Add(lvi);
                        }
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