using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;
using System.Reflection;

namespace StereoscopicImageViewer
{
    partial class AboutBox : Form
    {
        public AboutBox()
        {
            InitializeComponent();

            //  Initialize the AboutBox to display the product information from the assembly information.
            //  Change assembly information settings for your application through either:
            //  - Project->Properties->Application->Assembly Information
            //  - AssemblyInfo.cs
            this.Text = String.Format("About {0}", GetAssemblyInfo.AssemblyTitle);
            this.labelProductName.Text = GetAssemblyInfo.AssemblyProduct;
            this.labelVersion.Text = String.Format("Version {0}", GetAssemblyInfo.AssemblyVersion);
            this.labelCopyright.Text = GetAssemblyInfo.AssemblyCopyright;
            this.textBoxDescription.Text = GetAssemblyInfo.AssemblyDescription;
        }
    }
}
