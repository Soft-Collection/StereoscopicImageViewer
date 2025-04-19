namespace StereoscopicImageViewer
{
    partial class MainFrm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainFrm));
            this.popUpMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.hideOrShowToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.runAtStartupToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.alwaysOnTopToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.trayNotifyIcon = new System.Windows.Forms.NotifyIcon(this.components);
            this.menuStrip = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.visitWebSiteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.gitHubToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.arduinoProjectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.createImagesFromAnaglyphImageToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.stereoImagesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStrip = new System.Windows.Forms.ToolStrip();
            this.tsbOpenFolder = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.tscbComPort = new System.Windows.Forms.ToolStripComboBox();
            this.tsbRefresh = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
            this.tsbStartStop = new System.Windows.Forms.ToolStripButton();
            this.statusStrip = new System.Windows.Forms.StatusStrip();
            this.tsslFrequencyLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.tsslFrequency = new System.Windows.Forms.ToolStripStatusLabel();
            this.timerFrequency = new System.Windows.Forms.Timer(this.components);
            this.timerErrors = new System.Windows.Forms.Timer(this.components);
            this.fbdOpenFolder = new System.Windows.Forms.FolderBrowserDialog();
            this.pbVideoPanel = new System.Windows.Forms.PictureBox();
            this.lvStereoImages = new System.Windows.Forms.ListView();
            this.cmsStereoImages = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.refreshToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tbGlassesTimeOffset = new System.Windows.Forms.TrackBar();
            this.lblGlassesTimeOffset = new System.Windows.Forms.Label();
            this.tbTransparentTimePercent = new System.Windows.Forms.TrackBar();
            this.lblTransparentTimePercent = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.timerSendSettings = new System.Windows.Forms.Timer(this.components);
            this.openSelectedFolderInExplorerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.popUpMenu.SuspendLayout();
            this.menuStrip.SuspendLayout();
            this.toolStrip.SuspendLayout();
            this.statusStrip.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbVideoPanel)).BeginInit();
            this.cmsStereoImages.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.tbGlassesTimeOffset)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbTransparentTimePercent)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // popUpMenu
            // 
            this.popUpMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripSeparator1,
            this.hideOrShowToolStripMenuItem,
            this.runAtStartupToolStripMenuItem,
            this.alwaysOnTopToolStripMenuItem,
            this.helpToolStripMenuItem,
            this.exitToolStripMenuItem});
            this.popUpMenu.Name = "popUpMenu";
            this.popUpMenu.Size = new System.Drawing.Size(152, 120);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(148, 6);
            // 
            // hideOrShowToolStripMenuItem
            // 
            this.hideOrShowToolStripMenuItem.Name = "hideOrShowToolStripMenuItem";
            this.hideOrShowToolStripMenuItem.Size = new System.Drawing.Size(151, 22);
            this.hideOrShowToolStripMenuItem.Click += new System.EventHandler(this.hideOrShowToolStripMenuItem_Click);
            // 
            // runAtStartupToolStripMenuItem
            // 
            this.runAtStartupToolStripMenuItem.Name = "runAtStartupToolStripMenuItem";
            this.runAtStartupToolStripMenuItem.Size = new System.Drawing.Size(151, 22);
            this.runAtStartupToolStripMenuItem.Text = "Run at Startup";
            this.runAtStartupToolStripMenuItem.Click += new System.EventHandler(this.runAtStartupToolStripMenuItem_Click);
            // 
            // alwaysOnTopToolStripMenuItem
            // 
            this.alwaysOnTopToolStripMenuItem.Name = "alwaysOnTopToolStripMenuItem";
            this.alwaysOnTopToolStripMenuItem.Size = new System.Drawing.Size(151, 22);
            this.alwaysOnTopToolStripMenuItem.Text = "Always on Top";
            this.alwaysOnTopToolStripMenuItem.Click += new System.EventHandler(this.alwaysOnTopToolStripMenuItem_Click);
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(151, 22);
            this.helpToolStripMenuItem.Text = "Help";
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(107, 22);
            this.aboutToolStripMenuItem.Text = "About";
            this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(151, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // trayNotifyIcon
            // 
            this.trayNotifyIcon.ContextMenuStrip = this.popUpMenu;
            this.trayNotifyIcon.Icon = ((System.Drawing.Icon)(resources.GetObject("trayNotifyIcon.Icon")));
            this.trayNotifyIcon.Text = "Show/Hide Alpha Stopper";
            this.trayNotifyIcon.Visible = true;
            this.trayNotifyIcon.MouseClick += new System.Windows.Forms.MouseEventHandler(this.trayNotifyIcon_MouseClick);
            // 
            // menuStrip
            // 
            this.menuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.helpToolStripMenuItem1});
            this.menuStrip.Location = new System.Drawing.Point(0, 0);
            this.menuStrip.Name = "menuStrip";
            this.menuStrip.Size = new System.Drawing.Size(783, 24);
            this.menuStrip.TabIndex = 26;
            this.menuStrip.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.exitToolStripMenuItem1});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // exitToolStripMenuItem1
            // 
            this.exitToolStripMenuItem1.Name = "exitToolStripMenuItem1";
            this.exitToolStripMenuItem1.Size = new System.Drawing.Size(92, 22);
            this.exitToolStripMenuItem1.Text = "Exit";
            this.exitToolStripMenuItem1.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // helpToolStripMenuItem1
            // 
            this.helpToolStripMenuItem1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.visitWebSiteToolStripMenuItem,
            this.gitHubToolStripMenuItem,
            this.arduinoProjectToolStripMenuItem,
            this.createImagesFromAnaglyphImageToolStripMenuItem,
            this.stereoImagesToolStripMenuItem,
            this.aboutToolStripMenuItem1});
            this.helpToolStripMenuItem1.Name = "helpToolStripMenuItem1";
            this.helpToolStripMenuItem1.Size = new System.Drawing.Size(44, 20);
            this.helpToolStripMenuItem1.Text = "Help";
            // 
            // visitWebSiteToolStripMenuItem
            // 
            this.visitWebSiteToolStripMenuItem.Name = "visitWebSiteToolStripMenuItem";
            this.visitWebSiteToolStripMenuItem.Size = new System.Drawing.Size(270, 22);
            this.visitWebSiteToolStripMenuItem.Text = "Visit Web Site";
            this.visitWebSiteToolStripMenuItem.Click += new System.EventHandler(this.visitWebSiteToolStripMenuItem_Click);
            // 
            // gitHubToolStripMenuItem
            // 
            this.gitHubToolStripMenuItem.Name = "gitHubToolStripMenuItem";
            this.gitHubToolStripMenuItem.Size = new System.Drawing.Size(270, 22);
            this.gitHubToolStripMenuItem.Text = "GitHub";
            this.gitHubToolStripMenuItem.Click += new System.EventHandler(this.gitHubToolStripMenuItem_Click);
            // 
            // arduinoProjectToolStripMenuItem
            // 
            this.arduinoProjectToolStripMenuItem.Name = "arduinoProjectToolStripMenuItem";
            this.arduinoProjectToolStripMenuItem.Size = new System.Drawing.Size(270, 22);
            this.arduinoProjectToolStripMenuItem.Text = "Arduino Project";
            this.arduinoProjectToolStripMenuItem.Click += new System.EventHandler(this.arduinoProjectToolStripMenuItem_Click);
            // 
            // createImagesFromAnaglyphImageToolStripMenuItem
            // 
            this.createImagesFromAnaglyphImageToolStripMenuItem.Name = "createImagesFromAnaglyphImageToolStripMenuItem";
            this.createImagesFromAnaglyphImageToolStripMenuItem.Size = new System.Drawing.Size(270, 22);
            this.createImagesFromAnaglyphImageToolStripMenuItem.Text = "Create Images From Anaglyph Image";
            this.createImagesFromAnaglyphImageToolStripMenuItem.Click += new System.EventHandler(this.createImagesFromAnaglyphImageToolStripMenuItem_Click);
            // 
            // stereoImagesToolStripMenuItem
            // 
            this.stereoImagesToolStripMenuItem.Name = "stereoImagesToolStripMenuItem";
            this.stereoImagesToolStripMenuItem.Size = new System.Drawing.Size(270, 22);
            this.stereoImagesToolStripMenuItem.Text = "Stereo Images";
            this.stereoImagesToolStripMenuItem.Click += new System.EventHandler(this.stereoImagesToolStripMenuItem_Click);
            // 
            // aboutToolStripMenuItem1
            // 
            this.aboutToolStripMenuItem1.Name = "aboutToolStripMenuItem1";
            this.aboutToolStripMenuItem1.Size = new System.Drawing.Size(270, 22);
            this.aboutToolStripMenuItem1.Text = "About";
            this.aboutToolStripMenuItem1.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
            // 
            // toolStrip
            // 
            this.toolStrip.AutoSize = false;
            this.toolStrip.ImageScalingSize = new System.Drawing.Size(24, 24);
            this.toolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tsbOpenFolder,
            this.toolStripSeparator2,
            this.tscbComPort,
            this.tsbRefresh,
            this.toolStripSeparator5,
            this.tsbStartStop});
            this.toolStrip.Location = new System.Drawing.Point(0, 24);
            this.toolStrip.Name = "toolStrip";
            this.toolStrip.Size = new System.Drawing.Size(783, 30);
            this.toolStrip.TabIndex = 27;
            this.toolStrip.Text = "toolStrip1";
            // 
            // tsbOpenFolder
            // 
            this.tsbOpenFolder.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tsbOpenFolder.Image = global::StereoscopicImageViewer.Properties.Resources.open;
            this.tsbOpenFolder.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tsbOpenFolder.Name = "tsbOpenFolder";
            this.tsbOpenFolder.Size = new System.Drawing.Size(28, 27);
            this.tsbOpenFolder.Text = "Open Stereo Images Folder";
            this.tsbOpenFolder.Click += new System.EventHandler(this.tsbOpenFolder_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 30);
            // 
            // tscbComPort
            // 
            this.tscbComPort.Name = "tscbComPort";
            this.tscbComPort.Size = new System.Drawing.Size(100, 30);
            this.tscbComPort.Sorted = true;
            this.tscbComPort.SelectedIndexChanged += new System.EventHandler(this.tscbComPort_SelectedIndexChanged);
            // 
            // tsbRefresh
            // 
            this.tsbRefresh.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tsbRefresh.Image = global::StereoscopicImageViewer.Properties.Resources.refresh;
            this.tsbRefresh.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tsbRefresh.Name = "tsbRefresh";
            this.tsbRefresh.Size = new System.Drawing.Size(28, 27);
            this.tsbRefresh.Text = "Refresh COM Ports";
            this.tsbRefresh.Click += new System.EventHandler(this.tsbRefresh_Click);
            // 
            // toolStripSeparator5
            // 
            this.toolStripSeparator5.Name = "toolStripSeparator5";
            this.toolStripSeparator5.Size = new System.Drawing.Size(6, 30);
            // 
            // tsbStartStop
            // 
            this.tsbStartStop.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tsbStartStop.Image = global::StereoscopicImageViewer.Properties.Resources.play;
            this.tsbStartStop.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tsbStartStop.Name = "tsbStartStop";
            this.tsbStartStop.Size = new System.Drawing.Size(28, 27);
            this.tsbStartStop.Text = "Start";
            this.tsbStartStop.Click += new System.EventHandler(this.tsbStartStop_Click);
            // 
            // statusStrip
            // 
            this.statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tsslFrequencyLabel,
            this.tsslFrequency});
            this.statusStrip.Location = new System.Drawing.Point(0, 493);
            this.statusStrip.Name = "statusStrip";
            this.statusStrip.Size = new System.Drawing.Size(783, 22);
            this.statusStrip.TabIndex = 29;
            this.statusStrip.Text = "statusStrip1";
            // 
            // tsslFrequencyLabel
            // 
            this.tsslFrequencyLabel.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tsslFrequencyLabel.Name = "tsslFrequencyLabel";
            this.tsslFrequencyLabel.Size = new System.Drawing.Size(68, 17);
            this.tsslFrequencyLabel.Text = "Frequency:";
            // 
            // tsslFrequency
            // 
            this.tsslFrequency.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tsslFrequency.Name = "tsslFrequency";
            this.tsslFrequency.Size = new System.Drawing.Size(29, 17);
            this.tsslFrequency.Text = "0Hz";
            // 
            // timerFrequency
            // 
            this.timerFrequency.Tick += new System.EventHandler(this.timerFrequency_Tick);
            // 
            // timerErrors
            // 
            this.timerErrors.Tick += new System.EventHandler(this.timerErrors_Tick);
            // 
            // pbVideoPanel
            // 
            this.pbVideoPanel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.pbVideoPanel.BackColor = System.Drawing.SystemColors.ControlDark;
            this.pbVideoPanel.Location = new System.Drawing.Point(198, 57);
            this.pbVideoPanel.Name = "pbVideoPanel";
            this.pbVideoPanel.Size = new System.Drawing.Size(579, 375);
            this.pbVideoPanel.TabIndex = 28;
            this.pbVideoPanel.TabStop = false;
            // 
            // lvStereoImages
            // 
            this.lvStereoImages.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.lvStereoImages.ContextMenuStrip = this.cmsStereoImages;
            this.lvStereoImages.HideSelection = false;
            this.lvStereoImages.Location = new System.Drawing.Point(5, 57);
            this.lvStereoImages.MultiSelect = false;
            this.lvStereoImages.Name = "lvStereoImages";
            this.lvStereoImages.Size = new System.Drawing.Size(187, 375);
            this.lvStereoImages.TabIndex = 0;
            this.lvStereoImages.UseCompatibleStateImageBehavior = false;
            this.lvStereoImages.View = System.Windows.Forms.View.List;
            this.lvStereoImages.SelectedIndexChanged += new System.EventHandler(this.lvStereoImages_SelectedIndexChanged);
            // 
            // cmsStereoImages
            // 
            this.cmsStereoImages.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openSelectedFolderInExplorerToolStripMenuItem,
            this.refreshToolStripMenuItem});
            this.cmsStereoImages.Name = "cmsStereoImages";
            this.cmsStereoImages.Size = new System.Drawing.Size(245, 70);
            // 
            // refreshToolStripMenuItem
            // 
            this.refreshToolStripMenuItem.Name = "refreshToolStripMenuItem";
            this.refreshToolStripMenuItem.Size = new System.Drawing.Size(113, 22);
            this.refreshToolStripMenuItem.Text = "Refresh";
            this.refreshToolStripMenuItem.Click += new System.EventHandler(this.refreshToolStripMenuItem_Click);
            // 
            // tbGlassesTimeOffset
            // 
            this.tbGlassesTimeOffset.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tbGlassesTimeOffset.AutoSize = false;
            this.tbGlassesTimeOffset.Location = new System.Drawing.Point(6, 19);
            this.tbGlassesTimeOffset.Maximum = 100;
            this.tbGlassesTimeOffset.Name = "tbGlassesTimeOffset";
            this.tbGlassesTimeOffset.Size = new System.Drawing.Size(517, 23);
            this.tbGlassesTimeOffset.TabIndex = 30;
            this.tbGlassesTimeOffset.Scroll += new System.EventHandler(this.tbGlassesTimeOffset_Scroll);
            // 
            // lblGlassesTimeOffset
            // 
            this.lblGlassesTimeOffset.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.lblGlassesTimeOffset.Font = new System.Drawing.Font("Microsoft Sans Serif", 15F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblGlassesTimeOffset.Location = new System.Drawing.Point(520, 16);
            this.lblGlassesTimeOffset.Name = "lblGlassesTimeOffset";
            this.lblGlassesTimeOffset.Size = new System.Drawing.Size(53, 23);
            this.lblGlassesTimeOffset.TabIndex = 31;
            this.lblGlassesTimeOffset.Text = "0";
            this.lblGlassesTimeOffset.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // tbTransparentTimePercent
            // 
            this.tbTransparentTimePercent.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tbTransparentTimePercent.AutoSize = false;
            this.tbTransparentTimePercent.Location = new System.Drawing.Point(6, 19);
            this.tbTransparentTimePercent.Maximum = 100;
            this.tbTransparentTimePercent.Name = "tbTransparentTimePercent";
            this.tbTransparentTimePercent.Size = new System.Drawing.Size(125, 23);
            this.tbTransparentTimePercent.TabIndex = 32;
            this.tbTransparentTimePercent.TickFrequency = 5;
            this.tbTransparentTimePercent.Value = 70;
            this.tbTransparentTimePercent.Scroll += new System.EventHandler(this.tbTransparentTimePercent_Scroll);
            // 
            // lblTransparentTimePercent
            // 
            this.lblTransparentTimePercent.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.lblTransparentTimePercent.Font = new System.Drawing.Font("Microsoft Sans Serif", 15F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblTransparentTimePercent.Location = new System.Drawing.Point(133, 16);
            this.lblTransparentTimePercent.Name = "lblTransparentTimePercent";
            this.lblTransparentTimePercent.Size = new System.Drawing.Size(48, 23);
            this.lblTransparentTimePercent.TabIndex = 33;
            this.lblTransparentTimePercent.Text = "0";
            this.lblTransparentTimePercent.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.tbGlassesTimeOffset);
            this.groupBox1.Controls.Add(this.lblGlassesTimeOffset);
            this.groupBox1.Location = new System.Drawing.Point(198, 438);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(579, 52);
            this.groupBox1.TabIndex = 34;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Glasses Time Offset";
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.groupBox2.Controls.Add(this.tbTransparentTimePercent);
            this.groupBox2.Controls.Add(this.lblTransparentTimePercent);
            this.groupBox2.Location = new System.Drawing.Point(5, 438);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(187, 52);
            this.groupBox2.TabIndex = 35;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Transparent Time Percent";
            // 
            // timerSendSettings
            // 
            this.timerSendSettings.Tick += new System.EventHandler(this.timerSendSettings_Tick);
            // 
            // openSelectedFolderInExplorerToolStripMenuItem
            // 
            this.openSelectedFolderInExplorerToolStripMenuItem.Name = "openSelectedFolderInExplorerToolStripMenuItem";
            this.openSelectedFolderInExplorerToolStripMenuItem.Size = new System.Drawing.Size(244, 22);
            this.openSelectedFolderInExplorerToolStripMenuItem.Text = "Open Selected Folder In Explorer";
            this.openSelectedFolderInExplorerToolStripMenuItem.Click += new System.EventHandler(this.openSelectedFolderInExplorerToolStripMenuItem_Click);
            // 
            // MainFrm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(783, 515);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.pbVideoPanel);
            this.Controls.Add(this.lvStereoImages);
            this.Controls.Add(this.statusStrip);
            this.Controls.Add(this.toolStrip);
            this.Controls.Add(this.menuStrip);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MinimumSize = new System.Drawing.Size(500, 400);
            this.Name = "MainFrm";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainFrm_FormClosing);
            this.Move += new System.EventHandler(this.MainFrm_Move);
            this.Resize += new System.EventHandler(this.MainFrm_Resize);
            this.popUpMenu.ResumeLayout(false);
            this.menuStrip.ResumeLayout(false);
            this.menuStrip.PerformLayout();
            this.toolStrip.ResumeLayout(false);
            this.toolStrip.PerformLayout();
            this.statusStrip.ResumeLayout(false);
            this.statusStrip.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbVideoPanel)).EndInit();
            this.cmsStereoImages.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.tbGlassesTimeOffset)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbTransparentTimePercent)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ContextMenuStrip popUpMenu;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem hideOrShowToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem runAtStartupToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem alwaysOnTopToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.NotifyIcon trayNotifyIcon;
        private System.Windows.Forms.MenuStrip menuStrip;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem1;
        private System.Windows.Forms.ToolStrip toolStrip;
        private System.Windows.Forms.ToolStripButton tsbStartStop;
        private System.Windows.Forms.ToolStripMenuItem visitWebSiteToolStripMenuItem;
        private System.Windows.Forms.StatusStrip statusStrip;
        private System.Windows.Forms.ToolStripStatusLabel tsslFrequencyLabel;
        private System.Windows.Forms.ToolStripStatusLabel tsslFrequency;
        private System.Windows.Forms.Timer timerFrequency;
        private System.Windows.Forms.ToolStripButton tsbOpenFolder;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator5;
        private System.Windows.Forms.ToolStripComboBox tscbComPort;
        private System.Windows.Forms.Timer timerErrors;
        private System.Windows.Forms.ToolStripButton tsbRefresh;
        private System.Windows.Forms.FolderBrowserDialog fbdOpenFolder;
        private System.Windows.Forms.PictureBox pbVideoPanel;
        private System.Windows.Forms.ListView lvStereoImages;
        private System.Windows.Forms.TrackBar tbGlassesTimeOffset;
        private System.Windows.Forms.Label lblGlassesTimeOffset;
        private System.Windows.Forms.TrackBar tbTransparentTimePercent;
        private System.Windows.Forms.Label lblTransparentTimePercent;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Timer timerSendSettings;
        private System.Windows.Forms.ToolStripMenuItem gitHubToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem arduinoProjectToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem stereoImagesToolStripMenuItem;
        private System.Windows.Forms.ContextMenuStrip cmsStereoImages;
        private System.Windows.Forms.ToolStripMenuItem refreshToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem createImagesFromAnaglyphImageToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openSelectedFolderInExplorerToolStripMenuItem;
    }
}

