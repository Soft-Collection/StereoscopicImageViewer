﻿using Microsoft.Win32;
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
        private static string LeftImagePathValue = string.Empty;
        private static string RightImagePathValue = string.Empty;
        private static clsSIMWrap.eFrequencies FrequencyValue = clsSIMWrap.eFrequencies.Default;
        private static clsSIMWrap.eSignalSources SignalSourceValue = clsSIMWrap.eSignalSources.ScreenSensor;
        private static string ComPortValue = "COM1";

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
        public static string LeftImagePath
        {
            get
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                if (key == null) key = Registry.CurrentUser.CreateSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                LeftImagePathValue = (string)key.GetValue("LeftImagePath", string.Empty);
                return (LeftImagePathValue);
            }
            set
            {
                LeftImagePathValue = value;
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct, true);
                key.SetValue("LeftImagePath", LeftImagePathValue);
            }
        }

        // Right Image Path
        public static string RightImagePath
        {
            get
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                if (key == null) key = Registry.CurrentUser.CreateSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                RightImagePathValue = (string)key.GetValue("RightImagePath", string.Empty);
                return (RightImagePathValue);
            }
            set
            {
                RightImagePathValue = value;
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct, true);
                key.SetValue("RightImagePath", RightImagePathValue);
            }
        }

        // Frequency
        public static clsSIMWrap.eFrequencies Frequency
        {
            get
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                if (key == null) key = Registry.CurrentUser.CreateSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                FrequencyValue = (clsSIMWrap.eFrequencies)key.GetValue("Frequency", (int)clsSIMWrap.eFrequencies.Default);
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
        public static clsSIMWrap.eSignalSources SignalSource
        {
            get
            {
                RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                if (key == null) key = Registry.CurrentUser.CreateSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
                SignalSourceValue = (clsSIMWrap.eSignalSources)key.GetValue("SignalSource", (int)clsSIMWrap.eSignalSources.ScreenSensor);
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
    }
}
