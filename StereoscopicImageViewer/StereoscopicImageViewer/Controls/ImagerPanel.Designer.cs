namespace StereoscopicImageViewer
{
    partial class ImagerPanel
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
            this.ttControls = new System.Windows.Forms.ToolTip(this.components);
            this.bPlayStop = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // ttControls
            // 
            this.ttControls.AutoPopDelay = 5000;
            this.ttControls.InitialDelay = 500;
            this.ttControls.ReshowDelay = 100;
            // 
            // bPlayStop
            // 
            this.bPlayStop.BackgroundImage = global::StereoscopicImageViewer.Properties.Resources.aplay;
            this.bPlayStop.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.bPlayStop.FlatAppearance.BorderSize = 0;
            this.bPlayStop.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.bPlayStop.Location = new System.Drawing.Point(0, 0);
            this.bPlayStop.Name = "bPlayStop";
            this.bPlayStop.Size = new System.Drawing.Size(26, 26);
            this.bPlayStop.TabIndex = 35;
            this.bPlayStop.UseVisualStyleBackColor = true;
            this.bPlayStop.Click += new System.EventHandler(this.bPlayStop_Click);
            this.bPlayStop.MouseEnter += new System.EventHandler(this.bPlayPause_MouseEnter);
            // 
            // ImagerPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.bPlayStop);
            this.Name = "ImagerPanel";
            this.Size = new System.Drawing.Size(26, 26);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ToolTip ttControls;
        private System.Windows.Forms.Button bPlayStop;
    }
}