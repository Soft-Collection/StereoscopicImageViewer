namespace StereoscopicImageViewer
{
    partial class COMPortPanel
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.bRefresh = new System.Windows.Forms.Button();
            this.cbComPort = new System.Windows.Forms.ComboBox();
            this.ttControls = new System.Windows.Forms.ToolTip(this.components);
            this.SuspendLayout();
            // 
            // bRefresh
            // 
            this.bRefresh.BackgroundImage = global::StereoscopicImageViewer.Properties.Resources.refresh;
            this.bRefresh.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.bRefresh.FlatAppearance.BorderSize = 0;
            this.bRefresh.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.bRefresh.Location = new System.Drawing.Point(106, 0);
            this.bRefresh.Name = "bRefresh";
            this.bRefresh.Size = new System.Drawing.Size(26, 26);
            this.bRefresh.TabIndex = 44;
            this.bRefresh.UseVisualStyleBackColor = true;
            this.bRefresh.Click += new System.EventHandler(this.bRefresh_Click);
            this.bRefresh.MouseEnter += new System.EventHandler(this.bRefresh_MouseEnter);
            // 
            // cbComPort
            // 
            this.cbComPort.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbComPort.FormattingEnabled = true;
            this.cbComPort.Location = new System.Drawing.Point(0, 3);
            this.cbComPort.Name = "cbComPort";
            this.cbComPort.Size = new System.Drawing.Size(100, 21);
            this.cbComPort.TabIndex = 43;
            this.cbComPort.SelectedIndexChanged += new System.EventHandler(this.cbComPort_SelectedIndexChanged);
            this.cbComPort.MouseEnter += new System.EventHandler(this.cbComPort_MouseEnter);
            // 
            // ttControls
            // 
            this.ttControls.AutoPopDelay = 5000;
            this.ttControls.InitialDelay = 500;
            this.ttControls.ReshowDelay = 100;
            // 
            // COMPortPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.bRefresh);
            this.Controls.Add(this.cbComPort);
            this.Name = "COMPortPanel";
            this.Size = new System.Drawing.Size(132, 26);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button bRefresh;
        private System.Windows.Forms.ComboBox cbComPort;
        private System.Windows.Forms.ToolTip ttControls;
    }
}