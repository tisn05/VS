using cdrPluginT;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Text = Corel.Interop.VGCore.Text;

namespace cdrPluginT
{
    public partial class frmCompressText : BaseForm
    {
        public frmCompressText()
        {
            InitializeComponent();
        }

        public frmCompressText(Corel.Interop.VGCore.Application app)
        {
            InitializeComponent();
            this.application = app;
        }

        private void button_Do_Click(object sender, EventArgs e)
        {
            StartEvent(true);
            String splitStr = textBox1.Text.Trim();
            int lenStr = splitStr.Length;
            int Compressibility = (int)numericUpDown1.Value;
            Text objtext = application.ActiveSelection.Shapes[1].Text;
            int countmax = objtext.Story.Length;
            this.Cursor = System.Windows.Forms.Cursors.WaitCursor;
            long isPressed = 0;
            for (int i = 0; i < countmax; i++)
            {
                String sstr = objtext.Story.Range(i, i + 1).WideText;
                for (int i2 = 0; i2 < lenStr - 1; i2++)
                {
                    if (sstr == splitStr.Substring(i2, 1))
                    {
                        objtext.FontPropertiesInRange[i + 2, 1].RangeKerning = Compressibility;
                        objtext.FontPropertiesInRange[i + 1, 1].RangeKerning = Compressibility / 2;
                        isPressed++;
                        break;
                    }
                }
            }
            EndEvent();
            this.Cursor = System.Windows.Forms.Cursors.Default;
        }
    }
}
