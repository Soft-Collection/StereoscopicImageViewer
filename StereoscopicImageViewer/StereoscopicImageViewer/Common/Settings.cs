using Microsoft.Win32;
using System;
using System.Drawing;
using System.Windows.Forms;

namespace StereoscopicImageViewer
{
    class Settings
    {
        private static Point LocationValue;
        private static Size SizeValue;
        private static bool VisibleValue = true;
        private static bool AlwaysOnTopValue = true;
        private static bool RunAtStartupValue = false;
        private static string FolderPathValue = string.Empty;
        private static clsStereoImageManagerWrap.eFrequencies FrequencyValue = clsStereoImageManagerWrap.eFrequencies.Default;
        private static clsStereoImageManagerWrap.eSignalSources SignalSourceValue = clsStereoImageManagerWrap.eSignalSources.ScreenSensor;
        private static string ComPortValue = "COM1";
        private static int GlassesTimeOffsetValue = 0;
        private static int TransparentTimePercentValue = 70;

        //Location Property.
        public static Point Location
        {
            get
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                if (key == null) key = Registry.CurrentUser.CreateSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                LocationValue = new Point((int)key.GetValue("Left", 0), (int)key.GetValue("Top", 0));
                return (LocationValue);
            }
            set
            {
                LocationValue = value;
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct, true);
                key.SetValue("Left", LocationValue.X);
                key.SetValue("Top", LocationValue.Y);
            }
        }

        //Size Property.
        public static Size Size
        {
            get
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                if (key == null) key = Registry.CurrentUser.CreateSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                SizeValue = new Size((int)key.GetValue("Width", 352), (int)key.GetValue("Height", 370));
                return (SizeValue);
            }
            set
            {
                SizeValue = value;
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct, true);
                key.SetValue("Width", SizeValue.Width);
                key.SetValue("Height", SizeValue.Height);
            }
        }

        //Visible Property.
        public static bool Visible
        {
            get
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                if (key == null) key = Registry.CurrentUser.CreateSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                VisibleValue = Boolean.Parse((string)key.GetValue("Visible", "True"));
                return (VisibleValue);
            }
            set
            {
                VisibleValue = value;
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct, true);
                key.SetValue("Visible", VisibleValue.ToString());
            }
        }

        //Always On Top Property.
        public static bool AlwaysOnTop
        {
            get
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                if (key == null) key = Registry.CurrentUser.CreateSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                AlwaysOnTopValue = Boolean.Parse((string)key.GetValue("AlwaysOnTop", "True"));
                return (AlwaysOnTopValue);
            }
            set
            {
                AlwaysOnTopValue = value;
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct, true);
                key.SetValue("AlwaysOnTop", AlwaysOnTopValue.ToString());
            }
        }

        //Run At Startup Property.
        public static bool RunAtStartup
        {
            get
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                if (key == null) key = Registry.CurrentUser.CreateSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                RunAtStartupValue = Boolean.Parse((string)key.GetValue("RunAtStartup", "False"));
                //If application executable file location has been changed.
                RegistryKey runKey = Registry.CurrentUser.OpenSubKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", true);
                if (RunAtStartupValue) runKey.SetValue(GetAssemblyInfo.AssemblyProduct, Application.ExecutablePath.ToString());
                else runKey.DeleteValue(GetAssemblyInfo.AssemblyProduct, false);
                return (RunAtStartupValue);
            }
            set
            {
                RunAtStartupValue = value;
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct, true);
                key.SetValue("RunAtStartup", RunAtStartupValue.ToString());
                RegistryKey runKey = Registry.CurrentUser.OpenSubKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", true);
                if (RunAtStartupValue) runKey.SetValue(GetAssemblyInfo.AssemblyProduct, Application.ExecutablePath.ToString());
                else runKey.DeleteValue(GetAssemblyInfo.AssemblyProduct, false);
            }
        }

        // Left Image Path
        public static string FolderPath
        {
            get
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                if (key == null) key = Registry.CurrentUser.CreateSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                FolderPathValue = (string)key.GetValue("FolderPath", string.Empty);
                return (FolderPathValue);
            }
            set
            {
                FolderPathValue = value;
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct, true);
                key.SetValue("FolderPath", FolderPathValue);
            }
        }

        // Frequency
        public static clsStereoImageManagerWrap.eFrequencies Frequency
        {
            get
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                if (key == null) key = Registry.CurrentUser.CreateSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                FrequencyValue = (clsStereoImageManagerWrap.eFrequencies)key.GetValue("Frequency", (int)clsStereoImageManagerWrap.eFrequencies.Default);
                return (FrequencyValue);
            }
            set
            {
                FrequencyValue = value;
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct, true);
                key.SetValue("Frequency", (int)FrequencyValue);
            }
        }

        // Signal Source
        public static clsStereoImageManagerWrap.eSignalSources SignalSource
        {
            get
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                if (key == null) key = Registry.CurrentUser.CreateSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                SignalSourceValue = (clsStereoImageManagerWrap.eSignalSources)key.GetValue("SignalSource", (int)clsStereoImageManagerWrap.eSignalSources.ScreenSensor);
                return (SignalSourceValue);
            }
            set
            {
                SignalSourceValue = value;
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct, true);
                key.SetValue("SignalSource", (int)SignalSourceValue);
            }
        }

        // Com Port
        public static string ComPort
        {
            get
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                if (key == null) key = Registry.CurrentUser.CreateSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                ComPortValue = (string)key.GetValue("ComPort", "COM1");
                return (ComPortValue);
            }
            set
            {
                ComPortValue = value;
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct, true);
                key.SetValue("ComPort", ComPortValue);
            }
        }

        //Glasses Time Offset
        public static int GlassesTimeOffset
        {
            get
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                if (key == null) key = Registry.CurrentUser.CreateSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                GlassesTimeOffsetValue = (int)key.GetValue("GlassesTimeOffset", 0);
                return (GlassesTimeOffsetValue);
            }
            set
            {
                GlassesTimeOffsetValue = value;
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct, true);
                key.SetValue("GlassesTimeOffset", GlassesTimeOffsetValue);
            }
        }

        //Transparent Time Percent
        public static int TransparentTimePercent
        {
            get
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                if (key == null) key = Registry.CurrentUser.CreateSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                TransparentTimePercentValue = (int)key.GetValue("TransparentTimePercent", 70);
                return (TransparentTimePercentValue);
            }
            set
            {
                TransparentTimePercentValue = value;
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct, true);
                key.SetValue("TransparentTimePercent", TransparentTimePercentValue);
            }
        }
    }
}
