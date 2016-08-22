using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace SM_Selection_CSharp
{
    public partial class Docker : UserControl
    {
        Corel.Interop.VGCore.Application appDRAW = null;

        public Docker()
        {
            InitializeComponent();
        }

        public Docker(object app)
        {
            InitializeComponent();
            appDRAW = (Corel.Interop.VGCore.Application)app;
            //appDRAW.SelectionChange += new Corel.Interop.VGCore.DIDrawApplicationEvents_SelectionChangeEventHandler(SelectionChange);
            appDRAW.SelectionChange += new Corel.Interop.VGCore.DIVGApplicationEvents_SelectionChangeEventHandler(SelectionChange);
        }

        private void SelectionChange()
        {
            string strObjectName = "";

            if (appDRAW.ActiveShape != null)
            {
                strObjectName = appDRAW.ActiveShape.Evaluate("@type");
            }

            if (appDRAW.ActiveSelection != null)
            {
                txtNumSelected.Text = appDRAW.ActiveSelection.Shapes.Count.ToString();
            }

            txtObjectType.Text = strObjectName;
        }

        private void SelectOfType(string strType)
        {
            string strQuery = null;
            Corel.Interop.VGCore.ShapeRange srGroup = default(Corel.Interop.VGCore.ShapeRange);
            Corel.Interop.VGCore.ShapeRange srTopOnly = default(Corel.Interop.VGCore.ShapeRange);

            strQuery = "@type='" + strType + "'";
            srGroup = appDRAW.ActivePage.Shapes.FindShapes("", 0, true, strQuery);
            srTopOnly = appDRAW.ActivePage.Shapes.FindShapes("", 0, false, strQuery);
            srTopOnly.CreateSelection();
            appDRAW.ActivePage.Shapes.FindShapes("", 0, false, strQuery).CreateSelection();

            if (srTopOnly.Count == srGroup.Count)
            {
                lblWarning.Visibility = System.Windows.Visibility.Hidden;
            }
            else
            {
                lblWarning.Visibility = System.Windows.Visibility.Visible;
            }
        }

        private void btnRectangles_Click(object sender, RoutedEventArgs e)
        {
            SelectOfType("rectangle");
        }

        private void btnEllipses_Click(object sender, RoutedEventArgs e)
        {
            SelectOfType("ellipse");
        }

        private void btnCurves_Click(object sender, RoutedEventArgs e)
        {
            SelectOfType("curve");
        }

        private void btnPolygons_Click(object sender, RoutedEventArgs e)
        {
            SelectOfType("polygon");
        }

        private void btnArtisticText_Click(object sender, RoutedEventArgs e)
        {
            SelectOfType("text:artistic");
        }

        private void btnParagraphText_Click(object sender, RoutedEventArgs e)
        {
            SelectOfType("text:paragraph");
        }

        private void btnGroups_Click(object sender, RoutedEventArgs e)
        {
            SelectOfType("group");
        }

        private void btnSelectSame_Click(object sender, RoutedEventArgs e)
        {
            SelectOfType(txtObjectType.Text);
        }
    }
}
