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

namespace cdrPluginT
{
    public partial class frmColorReplace : BaseForm
    {
        private clsConfig cfg;
        Palette pal1, pal2;
        public frmColorReplace()
        {
            InitializeComponent();

        }

        public frmColorReplace(Corel.Interop.VGCore.Application app)
        {
            this.application = app;
            InitializeComponent();
        }

        private void frmColorReplace_Load(object sender, EventArgs e)
        {
            cfg = new clsConfig(application);
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
            sr = application.ActiveSelectionRange;
            if (application.ActiveDocument.GetUserClick(out X, out Y, out Shift, 100, true, cdrCursorShape.cdrCursorEyeDrop) != 0)
            {
                this.Show();
                return;
            }
            this.Show();
            Color cc = application.ActiveDocument.SampleColorAtPoint(X, Y);
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
            lpChange(listView1, cb1, pal1);
        }

        private void lpChange(ListView lb, ComboBox cb, Palette pal)
        {
            switch (cb.SelectedIndex)
            {
                case 0:
                    if (application.ActiveSelectionRange.Count == 0)
                    {
                        return;
                    }
                    else
                    {
                        pal = CreatePaletteFromSelection();
                    }
                    break;
                case 1:
                    pal = CreatePaletteFromDocument();
                    break;
                default:
                    pal = application.ActivePalette;
                    break;
            }
            fillPaletteFrame(lb, pal);
        }

        private Palette CreatePaletteFromSelection()
        {
            Palette pal;
            pal = application.Palettes.CreateFromSelection("Selected in " + application.ActiveDocument.Name, Overwrite: true);
            foreach (Shape s in application.ActiveSelection.Shapes.FindShapes(Type: cdrShapeType.cdrBitmapShape))
            {
                if (s.Bitmap.Mode == cdrImageType.cdrDuotoneImage)
                {
                    for (int i = 0; i < s.Bitmap.Duotone.InkCount; i++)
                    {
                        pal.AddColor(s.Bitmap.Duotone.Inks[i].Color);
                    }
                }
            }
            return pal;
        }

        private Palette CreatePaletteFromDocument()
        {
            Palette pal;
            pal = application.Palettes.CreateFromDocument("Colors in " + application.ActiveDocument.Name, Overwrite: true);
            foreach (Page p in application.ActiveDocument.Pages)
            {
                foreach (Shape s in p.Shapes.FindShapes(Type: cdrShapeType.cdrBitmapShape))
                {
                    if (s.Bitmap.Mode == cdrImageType.cdrDuotoneImage)
                    {
                        for (int i = 0; i < s.Bitmap.Duotone.InkCount; i++)
                        {
                            pal.AddColor(s.Bitmap.Duotone.Inks[i].Color);
                        }
                    }
                }
            }
            return pal;
        }

        private void fillPaletteFrame(ListView lb, Palette pal)
        {
            Color C;
            int w;
            ComboBox cbb;
            if (lb.Tag == pal.Name)
                return;
            C = new Color();
            w = lb.Width;
            int cols = cfg.cols;
            int size = w / cols;
            int sizev = size;
            if (cols == 1)
            {
                sizev = 20;
            }
            int i = (pal.Colors().Count / cols + 1) * size;
            lb.Controls.Clear();
            if (lb.Name == "listBox1")
            {
                cbb = cbF;
            }
            else
                cbb = cbR;
            cbb.Items.Clear();
            Button bt1 = new Button();
            bt1.Left = i % cols * size;
            bt1.Top = i / cols * sizev;
            bt1.Width = size;
            bt1.Height = sizev;
            bt1.BackColor = Color2.White;
            lb.Controls.Add(bt1);
            cbb.Items.Add("无颜色");
            foreach (Color cp in pal.Colors())
            {
                C.CopyAssign(cp);
                if (C.Name != "unnamed color")
                {
                    cbb.Items.Add(C.Name);
                }
                else
                {
                    cbb.Items.Add(C.Name);
                }
                C.ConvertToRGB();
                Button bt2 = new Button();
                //bt2.Left = i % cols * size;
                //bt2.Top = i / cols * sizev;
                bt2.Location = new System.Drawing.Point(5, 5);
                bt2.Width = size + 1;
                bt2.Height = sizev;
                bt2.BackColor = Color2.FromArgb(C.RGBRed, C.RGBGreen, C.RGBBlue);
                if (cols > 1)
                {
                    bt2.Enabled = false;
                }
                else
                {
                    bt2.Text = cp.Name;
                    bt2.TextAlign = ContentAlignment.MiddleCenter;
                    C.ConvertToGray();
                    if (C.Gray < 150)
                    {
                        bt2.ForeColor = Color2.White;
                    }
                }
                lb.Controls.Add(bt2);
                panel1.Controls.Add(bt2);
                bt2.Location = new System.Drawing.Point(i % cols * size, i / cols * sizev);
                i++;
            }
            lb.Tag = pal.Name;
            panel1.Refresh();
        }

    }
}
