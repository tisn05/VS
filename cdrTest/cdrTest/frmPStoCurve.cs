using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Corel.Interop.VGCore;

namespace cdrTest
{
    public partial class frmPStoCurve : BaseForm
    {
        public frmPStoCurve()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            ShapeRange OrigSelection = app.ActiveSelectionRange;
            ExportFilter expflt;
            StructExportOptions expopt = app.CreateStructExportOptions();
            expopt.UseColorProfile = false;
            expopt.ResolutionX = 72;
            expopt.ResolutionY = 72;
            expopt.ImageType = cdrImageType.cdrCMYKColorImage;
            StructPaletteOptions exppopt = app.CreateStructPaletteOptions();
            expflt = app.ActiveDocument.ExportEx("C:\\PStoCurve.eps", cdrFilter.cdrEPS, cdrExportRange.cdrSelection, expopt, exppopt);
            
            expflt.Finish();
            ImportFilter impflt;
            StructImportOptions impopt = app.CreateStructImportOptions();
            impopt.MaintainLayers = true;
            impflt = app.ActiveLayer.ImportEx("C:\\PStoCurve.eps", cdrFilter.cdrPSInterpreted, impopt);
            impflt.Finish();
            OrigSelection.Delete();
        }
    }
}
