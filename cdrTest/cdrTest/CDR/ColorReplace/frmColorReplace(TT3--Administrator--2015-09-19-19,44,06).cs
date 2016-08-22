using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Corel.Interop.VGCore;
using Color = Corel.Interop.VGCore.Color;
using Color2 = System.Drawing.Color;

namespace cdrTest
{
    public partial class frmColorReplace : BaseForm
    {
        private clsConfig cfg;
        public frmColorReplace()
        {
            InitializeComponent();

        }

        private void frmColorReplace_Load(object sender, EventArgs e)
        {
            cfg = new clsConfig(app);
            Button bt1 = new Button();
            bt1.Width = 30;
            bt1.Height = 30;
            listView1.Controls.Add(bt1);
            listView1.Controls.Add(bt1);
            listView1.Controls.Add(bt1);
            listView1.Controls.Add(bt1);
            listView1.Controls.Add(bt1);
            listView1.Controls.Add(bt1);
            listView1.Controls.Add(bt1);
            Color cRGB;
            cfg.Load();
            UpdateFind();
            UpdateReplace();
        }

        private void fillPalettes(ComboBox cb)
        {
            int ind = cb.SelectedIndex;
            cb.Items.Clear();
            cb.Items.Add("从选取对象中");
            cb.Items.Add("从文档中");
        }

        private void bt_Eyedropper_Click(object sender, EventArgs e)
        {
            Shape sh, sh2;
            double X, Y;
            int Shift;
            ShapeRange sr;
            bool stat;
            this.Hide();
            sr = app.ActiveSelectionRange;
            if(app.ActiveDocument.GetUserClick(out X,out Y,out Shift,100,true,cdrCursorShape.cdrCursorEyeDrop)!=0){
                this.Show();
                return;
            }
            this.Show();
            Color cc = app.ActiveDocument.SampleColorAtPoint(X, Y);
            /*sh = app.ActivePage.SelectShapesAtPoint(X, Y, false);
            if (sh.Shapes.Count == 0)
            {
                return;
            }
            sh = sh.Shapes[sh.Shapes.Count];
            if (sh.Type == cdrShapeType.cdrGroupShape)
            {
                StartEvent();
                sh.UngroupAll();
                sh2 = app.ActivePage.SelectShapesAtPoint(X, Y, false);
                stat = true;
                if (sh2.Shapes.Count > 0)
                {
                    sh = sh2.Shapes[sh2.Shapes.Count];
                }
                EndEvent();
            }
            app.ActiveDocument.ClearSelection();
            sr.CreateSelection();
            if ((Shift & 1) == 0)
            {
                if (sh.Fill.Type != cdrFillType.cdrUniformFill)
                {
                    return;
                }
                cfg.clsFind.CopyAssign(sh.Fill.UniformColor);
            }
            else
            {
                if (sh.Outline.Type != cdrOutlineType.cdrOutline)
                    return;
                cfg.clsFind.CopyAssign(sh.Outline.Color);
            }*/
            cfg.clsFind.CopyAssign(cc);
            UpdateFind();
        }

        private void UpdateFind()
        {
            UpdateColor(cfg.clsFind, cmFind, lblFind, cfg.fNoColor);
        }

        private void UpdateColor(Color C, Button btn, Label lbl, bool NoColor)
        {
            String sName;
            if (NoColor)
            {
                sName = "无颜色";
                btn.BackColor = Color2.Black;
            }
            else
            {
                Color cRGB = new Color();
                btn.Image = null;
                cRGB.CopyAssign(C);
                cRGB.ConvertToRGB();
                btn.BackColor = Color2.FromArgb(cRGB.RGBRed, cRGB.RGBGreen, cRGB.RGBBlue);
                sName = C.Name + "(" + C.ColorStyleName + ")";
            }
            lbl.Text = sName;
        }

        private void UpdateReplace()
        {

        }

        private void cb1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
        
    }
}
