using System;
using System.Threading;
using System.Windows.Forms;

namespace StereoscopicImageViewer
{
    static class Program
    {
        //Mutex can be made static so that GC doesn't recycle same effect with GC.KeepAlive(mutex) at the end of main.
        static Mutex mMutex = new Mutex(false, GetAssemblyInfo.AssemblyProduct);
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            //If you like to wait a few seconds in case that the instance is just shutting down.
            if (!mMutex.WaitOne(TimeSpan.FromSeconds(0), false))
            {
                MessageBox.Show("Application already started!", "", MessageBoxButtons.OK);
                return;
            }
            try
            {
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);
                Application.Run(new MainFrm());
            }
            finally
            {
                mMutex.ReleaseMutex();
            }
        }
    }
}
