using Corel.Interop.VGCore;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Color = Corel.Interop.VGCore.Color;

namespace cdrPluginT
{
    public partial class frmSelect : BaseForm
    {
        public frmSelect()
        {
            InitializeComponent();
        }

        public frmSelect(Corel.Interop.VGCore.Application app)
        {
            InitializeComponent();
            this.application = app;
            this.cmdName = "同类选择";
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Color RmbC, RmbCL;
            Shape S;
            long STT, STT1;
            double CT, CN, CT1, CN1;
            cdrShapeType STy;
            String FNT = "";
            Single Fsize = 0;
            StartEvent(true, false);
            RmbC = application.CreateColor();
            RmbCL = application.CreateColor();
            application.ActiveDocument.ReferencePoint = Corel.Interop.VGCore.cdrReferencePoint.cdrTopLeft;
            application.ActiveDocument.Unit = Corel.Interop.VGCore.cdrUnit.cdrMillimeter;
            if (application.ActiveDocument.Selection().Shapes.Count == 1)
            {
                S = application.ActiveSelection;
                S.GetSize(out CT, out CN);
                STy = S.Shapes[1].Type;
                STT = S.Shapes[1].Shapes.Count;
                if (S.Shapes[1].Fill.Type == cdrFillType.cdrUniformFill)
                {
                    RmbC.CopyAssign(application.ActiveShape.Fill.UniformColor);
                }
                if (S.Shapes[1].Outline.Type == cdrOutlineType.cdrOutline)
                {
                    RmbCL.CopyAssign(application.ActiveShape.Outline.Color);
                }
                if (S.Shapes[1].Type == cdrShapeType.cdrTextShape)
                {
                    FNT = S.Shapes[1].Text.Story.Font;
                    Fsize = S.Shapes[1].Text.Story.Size;
                }
                foreach (Shape s1 in application.ActiveDocument.SelectableShapes.FindShapes())
                {
                    if (s1.ParentGroup == null)
                    {
                        s1.GetSize(out CT1, out CN1);
                        STT1 = s1.Shapes.Count;
                        if (checkBox_SSize.Checked)
                        {
                            if (Math.Abs(CN1 - CN) <= 0.1 && Math.Abs(CT1 - CT) <= 0.1 && STT1 == STT)
                            {
                                s1.AddToSelection();
                            }
                            else if (CT * CN == CT1 * CN1 && STT1 == STT)
                            {
                                s1.AddToSelection();
                            }
                        }
                        if (checkBox_SType.Checked && s1.Type == STy)
                        {
                            s1.AddToSelection();
                        }
                        if (checkBox_SLines.Checked && s1.Type == cdrShapeType.cdrCurveShape && s1.Outline.Width == S.Outline.Width)
                        {
                            s1.AddToSelection();
                        }
                        if (checkBox_SColor.Checked && s1.Fill.Type == cdrFillType.cdrUniformFill && s1.Fill.UniformColor.IsSame(RmbC))
                        {
                            s1.AddToSelection();
                        }
                        if (checkBox_SFont.Checked && s1.Type == cdrShapeType.cdrTextShape && s1.Text.Story.Font == FNT)
                        {
                            s1.AddToSelection();
                        }
                        if (checkBox_SFontSize.Checked && s1.Type == cdrShapeType.cdrTextShape && s1.Text.Story.Size == Fsize)
                        {
                            s1.AddToSelection();
                        }
                    }
                }
            }
            else
            {
                MessageBox.Show("请选择一个对象或群组对象！");
            }
            EndEvent();
        }

        private void frmSelect_Load(object sender, EventArgs e)
        {

        }
   
    }
}
