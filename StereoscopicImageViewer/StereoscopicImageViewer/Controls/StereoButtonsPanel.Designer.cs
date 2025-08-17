namespace StereoscopicImageViewer
{
    partial class StereoButtonsPanel
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
            this.bSwap = new System.Windows.Forms.Button();
            this.bBoth = new System.Windows.Forms.Button();
            this.bRightOnly = new System.Windows.Forms.Button();
            this.bLeftOnly = new System.Windows.Forms.Button();
            this.ttControls = new System.Windows.Forms.ToolTip(this.components);
            this.SuspendLayout();
            // 
            // bSwap
            // 
            this.bSwap.BackgroundImage = global::StereoscopicImageViewer.Properties.Resources.swap_unselected;
            this.bSwap.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.bSwap.FlatAppearance.BorderSize = 0;
            this.bSwap.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.bSwap.Location = new System.Drawing.Point(87, 0);
            this.bSwap.Name = "bSwap";
            this.bSwap.Size = new System.Drawing.Size(26, 26);
            this.bSwap.TabIndex = 49;
            this.bSwap.UseVisualStyleBackColor = true;
            this.bSwap.Click += new System.EventHandler(this.bSwap_Click);
            this.bSwap.MouseEnter += new System.EventHandler(this.bSwap_MouseEnter);
            // 
            // bBoth
            // 
            this.bBoth.BackgroundImage = global::StereoscopicImageViewer.Properties.Resources.both_unselected;
            this.bBoth.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.bBoth.FlatAppearance.BorderSize = 0;
            this.bBoth.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.bBoth.Location = new System.Drawing.Point(0, 0);
            this.bBoth.Name = "bBoth";
            this.bBoth.Size = new System.Drawing.Size(26, 26);
            this.bBoth.TabIndex = 48;
            this.bBoth.UseVisualStyleBackColor = true;
            this.bBoth.Click += new System.EventHandler(this.bBoth_Click);
            this.bBoth.MouseEnter += new System.EventHandler(this.bBoth_MouseEnter);
            // 
            // bRightOnly
            // 
            this.bRightOnly.BackgroundImage = global::StereoscopicImageViewer.Properties.Resources.right_only_unselected;
            this.bRightOnly.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.bRightOnly.FlatAppearance.BorderSize = 0;
            this.bRightOnly.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.bRightOnly.Location = new System.Drawing.Point(58, 0);
            this.bRightOnly.Name = "bRightOnly";
            this.bRightOnly.Size = new System.Drawing.Size(26, 26);
            this.bRightOnly.TabIndex = 47;
            this.bRightOnly.UseVisualStyleBackColor = true;
            this.bRightOnly.Click += new System.EventHandler(this.bRightOnly_Click);
            this.bRightOnly.MouseEnter += new System.EventHandler(this.bRightOnly_MouseEnter);
            // 
            // bLeftOnly
            // 
            this.bLeftOnly.BackgroundImage = global::StereoscopicImageViewer.Properties.Resources.left_only_unselected;
            this.bLeftOnly.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.bLeftOnly.FlatAppearance.BorderSize = 0;
            this.bLeftOnly.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.bLeftOnly.Location = new System.Drawing.Point(29, 0);
            this.bLeftOnly.Name = "bLeftOnly";
            this.bLeftOnly.Size = new System.Drawing.Size(26, 26);
            this.bLeftOnly.TabIndex = 46;
            this.bLeftOnly.UseVisualStyleBackColor = true;
            this.bLeftOnly.Click += new System.EventHandler(this.bLeftOnly_Click);
            this.bLeftOnly.MouseEnter += new System.EventHandler(this.bLeftOnly_MouseEnter);
            // 
            // ttControls
            // 
            this.ttControls.AutoPopDelay = 5000;
            this.ttControls.InitialDelay = 500;
            this.ttControls.ReshowDelay = 100;
            // 
            // StereoButtonsPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.bSwap);
            this.Controls.Add(this.bBoth);
            this.Controls.Add(this.bRightOnly);
            this.Controls.Add(this.bLeftOnly);
            this.Name = "StereoButtonsPanel";
            this.Size = new System.Drawing.Size(113, 26);
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.Button bSwap;
        private System.Windows.Forms.Button bBoth;
        private System.Windows.Forms.Button bRightOnly;
        private System.Windows.Forms.Button bLeftOnly;
        private System.Windows.Forms.ToolTip ttControls;
    }
}