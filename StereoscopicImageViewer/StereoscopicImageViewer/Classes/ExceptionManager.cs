using System;
using System.IO;
using System.Text;
using System.Windows.Forms;

public class ExceptionManager
{
    #region Variables
    private static string logFileName = Application.StartupPath + "\\Report.log";
    private static TextWriter textWriterValue;
    #endregion

    #region Methods
    public static void Publish(Exception ex)
    {
        OpenLogFile();
        // Create StringBuilder to maintain publishing information.
        StringBuilder strInfo = new StringBuilder();
        // Record General information.
        strInfo.AppendFormat("{0}------------------------------- {1} -------------------------------{0} ", Environment.NewLine, DateTime.Now);
        strInfo.AppendFormat("{0}General Information:{0}", Environment.NewLine);
        strInfo.AppendFormat("{0}Additional Info:", Environment.NewLine);
        strInfo.AppendFormat("{0}{0}Exception Information{0}{1}", Environment.NewLine, ex.ToString());
        strInfo.AppendFormat("{0}{0}---------------------------------------------------- {0} ", Environment.NewLine);
        textWriterValue.Write(strInfo.ToString());
        // Update the underlying file.
        textWriterValue.Flush();
        CloseLogFile();
    }

    private static void OpenLogFile()
    {
        if (textWriterValue == null)
        {
            if (!Directory.Exists(Path.GetDirectoryName(logFileName)))
            {
                Directory.CreateDirectory(Path.GetDirectoryName(logFileName));
            }
            textWriterValue = new StreamWriter(logFileName, true);
            textWriterValue = TextWriter.Synchronized(textWriterValue);
        }
    }

    private static void CloseLogFile()
    {
        if (textWriterValue != null)
        {
            textWriterValue.Close();
            textWriterValue = null;
        }
    }
    #endregion
}
