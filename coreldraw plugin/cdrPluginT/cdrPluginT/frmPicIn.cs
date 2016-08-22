using Corel.Interop.VGCore;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace cdrPluginT
{
    public partial class frmPicIn : BaseForm
    {
        public frmPicIn()
        {
            InitializeComponent();
        }

        public frmPicIn(Corel.Interop.VGCore.Application app)
        {
            InitializeComponent();
            this.application = app;
            this.cmdName = "对象装框";
        }

        private void button1_Click(object sender, EventArgs e)
        {
            StartEvent(true);
            if (application.ActiveDocument.Selection().Shapes.Count == 0)
            {
                MessageBox.Show("请至少选择一个对象");
            }
            application.ActiveDocument.Unit = cdrUnit.cdrMillimeter;
            ShapeRange s = application.ActiveSelectionRange;
            Shape rec;
            double Radius = 0;
            if (checkBox1.Checked)
            {
                double.TryParse(textBox1.Text, out Radius);
            }
            rec = application.ActiveLayer.CreateRectangle2(s.LeftX, s.BottomY, s.SizeWidth, s.SizeHeight, Radius, Radius, Radius, Radius);
            rec.Rectangle.CornerType = cdrCornerType.cdrCornerTypeRound;
            rec.Rectangle.RelativeCornerScaling = true;
            rec.Fill.ApplyNoFill();
            s.AddToPowerClip(rec, cdrTriState.cdrFalse);
            EndEvent();
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            textBox1.Enabled = checkBox1.Checked;
        }

        private void frmPicIn_Load(object sender, EventArgs e)
        {
            checkBox1_CheckedChanged(null, null);
        }
    }
}
