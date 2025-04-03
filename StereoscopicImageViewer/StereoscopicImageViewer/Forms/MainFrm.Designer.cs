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
            this.aboutToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStrip = new System.Windows.Forms.ToolStrip();
            this.tsbOpenLeft = new System.Windows.Forms.ToolStripButton();
            this.tsbOpenRight = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
            this.tscbFrequency = new System.Windows.Forms.ToolStripComboBox();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripLabel3 = new System.Windows.Forms.ToolStripLabel();
            this.tscbSignalSource = new System.Windows.Forms.ToolStripComboBox();
            this.tscbComPort = new System.Windows.Forms.ToolStripComboBox();
            this.tsbRefresh = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
            this.tsbStartStop = new System.Windows.Forms.ToolStripButton();
            this.pbVideoPanel = new System.Windows.Forms.PictureBox();
            this.statusStrip = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.tsslFrequency = new System.Windows.Forms.ToolStripStatusLabel();
            this.timerFrequency = new System.Windows.Forms.Timer(this.components);
            this.dlgOpenImage = new System.Windows.Forms.OpenFileDialog();
            this.timerErrors = new System.Windows.Forms.Timer(this.components);
            this.popUpMenu.SuspendLayout();
            this.menuStrip.SuspendLayout();
            this.toolStrip.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbVideoPanel)).BeginInit();
            this.statusStrip.SuspendLayout();
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
            this.aboutToolStripMenuItem1});
            this.helpToolStripMenuItem1.Name = "helpToolStripMenuItem1";
            this.helpToolStripMenuItem1.Size = new System.Drawing.Size(44, 20);
            this.helpToolStripMenuItem1.Text = "Help";
            // 
            // visitWebSiteToolStripMenuItem
            // 
            this.visitWebSiteToolStripMenuItem.Name = "visitWebSiteToolStripMenuItem";
            this.visitWebSiteToolStripMenuItem.Size = new System.Drawing.Size(145, 22);
            this.visitWebSiteToolStripMenuItem.Text = "Visit Web Site";
            this.visitWebSiteToolStripMenuItem.Click += new System.EventHandler(this.visitWebSiteToolStripMenuItem_Click);
            // 
            // aboutToolStripMenuItem1
            // 
            this.aboutToolStripMenuItem1.Name = "aboutToolStripMenuItem1";
            this.aboutToolStripMenuItem1.Size = new System.Drawing.Size(145, 22);
            this.aboutToolStripMenuItem1.Text = "About";
            this.aboutToolStripMenuItem1.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
            // 
            // toolStrip
            // 
            this.toolStrip.AutoSize = false;
            this.toolStrip.ImageScalingSize = new System.Drawing.Size(24, 24);
            this.toolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tsbOpenLeft,
            this.tsbOpenRight,
            this.toolStripSeparator2,
            this.toolStripLabel1,
            this.tscbFrequency,
            this.toolStripSeparator4,
            this.toolStripLabel3,
            this.tscbSignalSource,
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
            // tsbOpenLeft
            // 
            this.tsbOpenLeft.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tsbOpenLeft.Image = global::StereoscopicImageViewer.Properties.Resources.openleft;
            this.tsbOpenLeft.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tsbOpenLeft.Name = "tsbOpenLeft";
            this.tsbOpenLeft.Size = new System.Drawing.Size(28, 27);
            this.tsbOpenLeft.Text = "Open Left Image";
            this.tsbOpenLeft.Click += new System.EventHandler(this.tsbOpenLeft_Click);
            // 
            // tsbOpenRight
            // 
            this.tsbOpenRight.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.tsbOpenRight.Image = global::StereoscopicImageViewer.Properties.Resources.openright;
            this.tsbOpenRight.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.tsbOpenRight.Name = "tsbOpenRight";
            this.tsbOpenRight.Size = new System.Drawing.Size(28, 27);
            this.tsbOpenRight.Text = "Open Right Image";
            this.tsbOpenRight.Click += new System.EventHandler(this.tsbOpenRight_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 30);
            // 
            // toolStripLabel1
            // 
            this.toolStripLabel1.Name = "toolStripLabel1";
            this.toolStripLabel1.Size = new System.Drawing.Size(62, 27);
            this.toolStripLabel1.Text = "Frequency";
            // 
            // tscbFrequency
            // 
            this.tscbFrequency.AutoCompleteCustomSource.AddRange(new string[] {
            "1Hz",
            "60Hz",
            "75Hz",
            "100Hz",
            "120Hz",
            "144Hz"});
            this.tscbFrequency.Items.AddRange(new object[] {
            "Default",
            "1Hz",
            "60Hz",
            "75Hz",
            "100Hz",
            "120Hz",
            "144Hz"});
            this.tscbFrequency.Name = "tscbFrequency";
            this.tscbFrequency.Size = new System.Drawing.Size(75, 30);
            this.tscbFrequency.SelectedIndexChanged += new System.EventHandler(this.tscbFrequency_SelectedIndexChanged);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(6, 30);
            // 
            // toolStripLabel3
            // 
            this.toolStripLabel3.Name = "toolStripLabel3";
            this.toolStripLabel3.Size = new System.Drawing.Size(78, 27);
            this.toolStripLabel3.Text = "Signal Source";
            // 
            // tscbSignalSource
            // 
            this.tscbSignalSource.Items.AddRange(new object[] {
            "Screen Sensor",
            "COM Port"});
            this.tscbSignalSource.Name = "tscbSignalSource";
            this.tscbSignalSource.Size = new System.Drawing.Size(100, 30);
            this.tscbSignalSource.SelectedIndexChanged += new System.EventHandler(this.tscbSignalSource_SelectedIndexChanged);
            // 
            // tscbComPort
            // 
            this.tscbComPort.Name = "tscbComPort";
            this.tscbComPort.Size = new System.Drawing.Size(100, 30);
            this.tscbComPort.Visible = false;
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
            // pbVideoPanel
            // 
            this.pbVideoPanel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.pbVideoPanel.BackColor = System.Drawing.SystemColors.ControlDark;
            this.pbVideoPanel.Location = new System.Drawing.Point(0, 57);
            this.pbVideoPanel.Name = "pbVideoPanel";
            this.pbVideoPanel.Size = new System.Drawing.Size(783, 433);
            this.pbVideoPanel.TabIndex = 28;
            this.pbVideoPanel.TabStop = false;
            // 
            // statusStrip
            // 
            this.statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1,
            this.tsslFrequency});
            this.statusStrip.Location = new System.Drawing.Point(0, 493);
            this.statusStrip.Name = "statusStrip";
            this.statusStrip.Size = new System.Drawing.Size(783, 22);
            this.statusStrip.TabIndex = 29;
            this.statusStrip.Text = "statusStrip1";
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(68, 17);
            this.toolStripStatusLabel1.Text = "Frequency:";
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
            // MainFrm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(783, 515);
            this.Controls.Add(this.statusStrip);
            this.Controls.Add(this.pbVideoPanel);
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
            ((System.ComponentModel.ISupportInitialize)(this.pbVideoPanel)).EndInit();
            this.statusStrip.ResumeLayout(false);
            this.statusStrip.PerformLayout();
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
        private System.Windows.Forms.PictureBox pbVideoPanel;
        private System.Windows.Forms.ToolStripButton tsbStartStop;
        private System.Windows.Forms.ToolStripMenuItem visitWebSiteToolStripMenuItem;
        private System.Windows.Forms.StatusStrip statusStrip;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.ToolStripStatusLabel tsslFrequency;
        private System.Windows.Forms.Timer timerFrequency;
        private System.Windows.Forms.ToolStripButton tsbOpenLeft;
        private System.Windows.Forms.ToolStripButton tsbOpenRight;
        private System.Windows.Forms.OpenFileDialog dlgOpenImage;
        private System.Windows.Forms.ToolStripLabel toolStripLabel1;
        private System.Windows.Forms.ToolStripComboBox tscbFrequency;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripLabel toolStripLabel3;
        private System.Windows.Forms.ToolStripComboBox tscbSignalSource;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator5;
        private System.Windows.Forms.ToolStripComboBox tscbComPort;
        private System.Windows.Forms.Timer timerErrors;
        private System.Windows.Forms.ToolStripButton tsbRefresh;
    }
}

