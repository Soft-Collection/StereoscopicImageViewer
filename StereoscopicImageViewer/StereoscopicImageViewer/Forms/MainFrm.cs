using System;
using System.IO;
using System.Diagnostics;
using System.Security.Permissions;
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
            InitStereoPlayer();
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
            pbVideoPanel.BackColor = System.Drawing.Color.DarkGray;
            //------------------------------------------------------
            tsslFrequencyLabel.Visible = false;
            tsslFrequency.Visible = false;
            //-----------------------------------
            //Apply these settings.
            SetVisible();
            SetAlwaysOnTop();
            SetRunAtStartup();
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
            PerformStereoStop();
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
                    isStarted = mStereoImageManager.StereoIsStarted();
                }
                mWasStartedWhenMinimized = isStarted;
                if (isStarted) PerformStereoStop();
            }
            else
            {
                if (mWasStartedWhenMinimized)
                {
                    bool isStarted = false;
                    if (mStereoImageManager != null)
                    {
                        isStarted = mStereoImageManager.StereoIsStarted();
                    }
                    if (!isStarted) PerformStereoStart();
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

        #region Enums
        #endregion

        #region Variables
        private bool mWasStartedWhenMinimized = false;
        private clsStereoImageManager mStereoImageManager = null;
        #endregion

        #region Initialize
        private void Init()
        {
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
            System.Diagnostics.Process.Start("https://github.com/Soft-Collection/StereoscopicImageViewer/tree/master/ArduinoProject/CableGlasses");
        }
        #endregion

        #region Main Form Events
        #endregion

        #region Methods
        #endregion

        #endregion

        #region Stereo Imager

        #region Enums
        private enum eMainStates
        {
            ImagesFolderNotOpened = 0,
            COMPortNotSelected = 1,
            Stopped = 2,
            Playing = 3
        }
        #endregion

        #region Variables
        private bool mAlreadySent = true;
        private int mAlreadySentCounter = 0;
        private long mLastWindowResizeTime = 0;
        private bool mResizeAlreadyApplied = true;
        private bool mLastIsDirectoryExists = false;
        private bool mLastIsCOMPortSelected = false;
        private eMainStates mMainState = eMainStates.ImagesFolderNotOpened;
        private eMainStates mLastMainState = eMainStates.ImagesFolderNotOpened;
        private string mLeftImagePath = null;
        private string mRightImagePath = null;
        #endregion

        #region Initialize
        private void InitStereoPlayer()
        {
            timerFrequency.Start();
            timerSendSettings.Start();
            timerGUIStereoPlayer.Start();
            SetMainState(eMainStates.ImagesFolderNotOpened);
            LoadSettingsToControls();
        }
        #endregion

        #region Menu Events
        #endregion

        #region Main Form Events
        private void MainFrm_DragEnter(object sender, DragEventArgs e)
        {
            Array data = ((IDataObject)e.Data).GetData("FileDrop") as Array;
            if (data != null)
            {
                if ((data.Length == 1) && (data.GetValue(0) is String))
                {
                    string folderpath = ((string[])data)[0];
                    if (Directory.Exists(folderpath))
                    {
                        e.Effect = DragDropEffects.Copy; //Will show small plus (+)
                    }
                    else
                    {
                        e.Effect = DragDropEffects.None; //Will show small prohibition symbol.
                    }
                }
                else
                {
                    e.Effect = DragDropEffects.None; //Will show small prohibition symbol.
                }
            }
        }
        private void MainFrm_DragDrop(object sender, DragEventArgs e)
        {
            Array data = ((IDataObject)e.Data).GetData("FileDrop") as Array;
            if (data != null)
            {
                if ((data.Length == 1) && (data.GetValue(0) is String))
                {
                    string folderpath = ((string[])data)[0];
                    if (Directory.Exists(folderpath))
                    {
                        if (Settings.FolderPath != folderpath)
                        {
                            Settings.FolderPath = folderpath;
                            LoadStereoImages();
                        }
                    }
                }
            }
        }
        private void timerGUIStereoPlayer_Tick(object sender, EventArgs e)
        {
            bool isDirectoryExists = Directory.Exists(Settings.FolderPath);
            if (mLastIsDirectoryExists != isDirectoryExists)
            {
                if (isDirectoryExists)
                {
                    SetMainState((cppComPortPanel.IsCOMPortSelected) ? mLastMainState : eMainStates.COMPortNotSelected);
                }
                else
                {
                    SetMainState(eMainStates.ImagesFolderNotOpened);
                }
                mLastIsDirectoryExists = isDirectoryExists;
            }
            //-----------------------------------------------------
            if (mLastIsCOMPortSelected != cppComPortPanel.IsCOMPortSelected)
            {
                if (!cppComPortPanel.IsCOMPortSelected)
                {
                    SetMainState(eMainStates.COMPortNotSelected);
                }
                mLastIsCOMPortSelected = cppComPortPanel.IsCOMPortSelected;
            }
            //-----------------------------------------------------
            if (((double)(Stopwatch.GetTimestamp() - mLastWindowResizeTime)) / ((double)Stopwatch.Frequency / 1000.0) > 200)
            {
                if (!mResizeAlreadyApplied)
                {
                    if (mStereoImageManager != null)
                    {
                        mStereoImageManager.StereoWindowSizeChanged();
                    }
                    mResizeAlreadyApplied = true;
                }
            }
        }
        private void timerFrequency_Tick(object sender, EventArgs e)
        {
            int frequencyInHz = 0;
            if (mStereoImageManager != null)
            {
                frequencyInHz = mStereoImageManager.StereoGetFrequency();
            }
            tsslFrequency.Text = frequencyInHz.ToString() + "Hz";
        }
        private void timerSendSettings_Tick(object sender, EventArgs e)
        {
            if (!mAlreadySent)
            {
                if (mAlreadySentCounter > 5)
                {
                    if (mStereoImageManager != null)
                    {
                        mStereoImageManager.StereoLRBoth(Settings.LRBoth);
                        mStereoImageManager.StereoSwapLR(Settings.SwapLR);
                        mStereoImageManager.StereoSetGlassesTimeOffset(Settings.GlassesTimeOffset);
                    }
                    mAlreadySent = true;
                }
                mAlreadySentCounter++;
            }
        }
        private void bOpen_Click(object sender, EventArgs e)
        {
            string temp = OpenFolder();
            Settings.FolderPath = (temp == string.Empty) ? Settings.FolderPath : temp;
            LoadStereoImages();
        }
        private void pbVideoPanel_SizeChanged(object sender, EventArgs e)
        {
            mLastWindowResizeTime = Stopwatch.GetTimestamp();
            mResizeAlreadyApplied = false;
        }
        private void lvStereoImages_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (lvStereoImages.SelectedItems.Count > 0)
            {
                mLeftImagePath = Settings.FolderPath + "\\" + ((Tuple<string, string>)lvStereoImages.SelectedItems[0].Tag).Item1;
                mRightImagePath = Settings.FolderPath + "\\" + ((Tuple<string, string>)lvStereoImages.SelectedItems[0].Tag).Item2;
                if (mStereoImageManager != null)
                {
                    mStereoImageManager.ImagerProvideImages(mLeftImagePath, mRightImagePath);
                }
            }
        }
        private void ppPlayerPanel_OnPlay(object sender, EventArgs e)
        {
            SetMainState(eMainStates.Playing);
            PerformStereoStart();
        }
        private void ppPlayerPanel_OnStop(object sender, EventArgs e)
        {
            PerformStereoStop();
            SetMainState(eMainStates.Stopped);
        }
        private void cppComPortPanel_OnCOMPortSelected(object sender, COMPortPanel.COMPortEventArgs e)
        {
            Settings.ComPort = e.COMPort;
            SetMainState(eMainStates.Stopped);
        }
        private void sbpStereoButtonsPanel_OnBoth(object sender, EventArgs e)
        {
            if (mStereoImageManager != null)
            {
                mStereoImageManager.StereoLRBoth(0);
                Settings.LRBoth = 0;
            }
        }
        private void sbpStereoButtonsPanel_OnLeftOnly(object sender, EventArgs e)
        {
            if (mStereoImageManager != null)
            {
                mStereoImageManager.StereoLRBoth(1);
                Settings.LRBoth = 1;
            }
        }
        private void sbpStereoButtonsPanel_OnRightOnly(object sender, EventArgs e)
        {
            if (mStereoImageManager != null)
            {
                mStereoImageManager.StereoLRBoth(2);
                Settings.LRBoth = 2;
            }
        }
        private void sbpStereoButtonsPanel_OnNoSwap(object sender, EventArgs e)
        {
            if (mStereoImageManager != null)
            {
                mStereoImageManager.StereoSwapLR(false);
                Settings.SwapLR = false;
            }
        }
        private void sbpStereoButtonsPanel_OnSwap(object sender, EventArgs e)
        {
            if (mStereoImageManager != null)
            {
                mStereoImageManager.StereoSwapLR(true);
                Settings.SwapLR = true;
            }
        }
        private void gtopGlassesTimeOffsetPanel_OnTimeOffset(object sender, GlassesTimeOffsetPanel.TimeOffsetEventArgs e)
        {
            if (mStereoImageManager != null)
            {
                mStereoImageManager.StereoSetGlassesTimeOffset(e.TimeOffset);
                Settings.GlassesTimeOffset = e.TimeOffset;
            }
        }
        private void spSplitContainer_SplitterMoved(object sender, SplitterEventArgs e)
        {
            Settings.Splitter = e.SplitX;
        }
        private void bOpen_MouseEnter(object sender, EventArgs e)
        {
            ttControls.Show("Open video file", (IWin32Window)sender, 5000);
        }
        #endregion

        #region Methods
        private void SetMainState(eMainStates mainState)
        {
            mLastMainState = mMainState;
            mMainState = mainState;
            switch (mainState)
            {
                case eMainStates.ImagesFolderNotOpened:
                    bOpen.Enabled = true;
                    lvStereoImages.Enabled = false;
                    ppPlayerPanel.Enabled = false;
                    cppComPortPanel.Enabled = false;
                    sbpStereoButtonsPanel.Enabled = false;
                    gtopGlassesTimeOffsetPanel.Enabled = false;
                    tsslFrequencyLabel.Visible = false;
                    tsslFrequency.Visible = false;
                    break;
                case eMainStates.COMPortNotSelected:
                    bOpen.Enabled = true;
                    lvStereoImages.Enabled = false;
                    ppPlayerPanel.Enabled = false;
                    cppComPortPanel.Enabled = true;
                    sbpStereoButtonsPanel.Enabled = false;
                    gtopGlassesTimeOffsetPanel.Enabled = false;
                    tsslFrequencyLabel.Visible = false;
                    tsslFrequency.Visible = false;
                    break;
                case eMainStates.Stopped:
                    bOpen.Enabled = true;
                    lvStereoImages.Enabled = false;
                    ppPlayerPanel.Enabled = true;
                    cppComPortPanel.Enabled = true;
                    sbpStereoButtonsPanel.Enabled = false;
                    gtopGlassesTimeOffsetPanel.Enabled = false;
                    tsslFrequencyLabel.Visible = false;
                    tsslFrequency.Visible = false;
                    break;
                case eMainStates.Playing:
                    bOpen.Enabled = true;
                    lvStereoImages.Enabled = true;
                    ppPlayerPanel.Enabled = true;
                    cppComPortPanel.Enabled = false;
                    sbpStereoButtonsPanel.Enabled = true;
                    gtopGlassesTimeOffsetPanel.Enabled = true;
                    tsslFrequencyLabel.Visible = true;
                    tsslFrequency.Visible = true;
                    break;
            }
        }
        private void LoadSettingsToControls()
        {
            spSplitContainer.SplitterDistance = Settings.Splitter;
            cppComPortPanel.SelectedCOMPort = Settings.ComPort;
            cppComPortPanel.LoadPorts();
            switch (Settings.LRBoth)
            {
                case 0:
                    sbpStereoButtonsPanel.LRBothButtonsState = StereoButtonsPanel.eLRBothButtonsStates.Both;
                    break;
                case 1:
                    sbpStereoButtonsPanel.LRBothButtonsState = StereoButtonsPanel.eLRBothButtonsStates.LeftOnly;
                    break;
                case 2:
                    sbpStereoButtonsPanel.LRBothButtonsState = StereoButtonsPanel.eLRBothButtonsStates.RightOnly;
                    break;
            }
            sbpStereoButtonsPanel.SwapButtonState = (Settings.SwapLR) ? StereoButtonsPanel.eSwapButtonStates.SwapOn : StereoButtonsPanel.eSwapButtonStates.SwapOff;
            gtopGlassesTimeOffsetPanel.TimeOffset = Settings.GlassesTimeOffset;
            LoadStereoImages();
        }
        private void PerformStereoStart()
        {
            if (mStereoImageManager != null)
            {
                bool isStereoStarted = false;
                isStereoStarted = mStereoImageManager.StereoIsStarted();
                //To Be Started here.
                if (!isStereoStarted)
                {
                    StereoStart();
                    mAlreadySent = false;
                    mAlreadySentCounter = 0;
                }
            }
        }
        private void PerformStereoStop()
        {
            if (mStereoImageManager != null)
            {
                bool isStarted = false;
                isStarted = mStereoImageManager.StereoIsStarted();
                //To Be Stopped here.
                if (isStarted)
                {
                    StereoStop();
                    pbVideoPanel.Refresh();
                }
            }
        }
        private void StereoStart()
        {
            if (mStereoImageManager != null)
            {
                mStereoImageManager.StereoSetCOMPort(Settings.ComPort);
                mStereoImageManager.StereoStart();
            }
        }
        private void StereoStop()
        {
            if (mStereoImageManager != null)
            {
                mStereoImageManager.StereoStop();
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
                List<string> supportedExtensions = new List<string> { ".PNG", ".png", ".JPEG", ".jpeg", ".JPG", ".jpg", ".BMP", ".bmp", ".TIFF", ".tiff", ".GIF", ".gif" };
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