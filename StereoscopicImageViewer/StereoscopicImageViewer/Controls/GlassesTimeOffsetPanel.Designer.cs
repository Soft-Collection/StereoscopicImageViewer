namespace StereoscopicImageViewer
{
    partial class GlassesTimeOffsetPanel
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
            this.tbGlassesTimeOffset = new System.Windows.Forms.TrackBar();
            this.lblGlassesTimeOffset = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.tbGlassesTimeOffset)).BeginInit();
            this.SuspendLayout();
            // 
            // ttControls
            // 
            this.ttControls.AutoPopDelay = 5000;
            this.ttControls.InitialDelay = 500;
            this.ttControls.ReshowDelay = 100;
            // 
            // tbGlassesTimeOffset
            // 
            this.tbGlassesTimeOffset.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tbGlassesTimeOffset.AutoSize = false;
            this.tbGlassesTimeOffset.Location = new System.Drawing.Point(0, 0);
            this.tbGlassesTimeOffset.Maximum = 100;
            this.tbGlassesTimeOffset.Name = "tbGlassesTimeOffset";
            this.tbGlassesTimeOffset.Size = new System.Drawing.Size(552, 23);
            this.tbGlassesTimeOffset.TabIndex = 32;
            this.tbGlassesTimeOffset.Scroll += new System.EventHandler(this.tbGlassesTimeOffset_Scroll);
            this.tbGlassesTimeOffset.MouseEnter += new System.EventHandler(this.tbGlassesTimeOffset_MouseEnter);
            // 
            // lblGlassesTimeOffset
            // 
            this.lblGlassesTimeOffset.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.lblGlassesTimeOffset.Font = new System.Drawing.Font("Microsoft Sans Serif", 15F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblGlassesTimeOffset.Location = new System.Drawing.Point(558, -3);
            this.lblGlassesTimeOffset.Name = "lblGlassesTimeOffset";
            this.lblGlassesTimeOffset.Size = new System.Drawing.Size(53, 23);
            this.lblGlassesTimeOffset.TabIndex = 33;
            this.lblGlassesTimeOffset.Text = "0";
            this.lblGlassesTimeOffset.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // GlassesTimeOffsetPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.tbGlassesTimeOffset);
            this.Controls.Add(this.lblGlassesTimeOffset);
            this.Name = "GlassesTimeOffsetPanel";
            this.Size = new System.Drawing.Size(615, 24);
            ((System.ComponentModel.ISupportInitialize)(this.tbGlassesTimeOffset)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ToolTip ttControls;
        private System.Windows.Forms.TrackBar tbGlassesTimeOffset;
        private System.Windows.Forms.Label lblGlassesTimeOffset;
    }
}