using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace CSharpWin
{
    /* 作者：Starts_2000
     * 日期：2009-09-08
     * 网站：http://www.csharpwin.com CS 程序员之窗。
     * 你可以免费使用或修改以下代码，但请保留版权信息。
     * 具体请查看 CS程序员之窗开源协议（http://www.csharpwin.com/csol.html）。
     */
    public class CaptureImageToolColorTable
    {
        private static readonly Color _borderColor = Color.FromArgb(105, 153, 255);
        private static readonly Color _backColorNormal = Color.FromArgb(240, 240, 240);
        private static readonly Color _backColorHover = Color.FromArgb(230, 230, 230);
        private static readonly Color _backColorPressed = Color.FromArgb(105, 153, 255);
        private static readonly Color _foreColor = Color.FromArgb(0, 0, 0);

        public CaptureImageToolColorTable() { }

        public virtual Color BorderColor
        {
            get { return _borderColor; }
        }

        public virtual Color BackColorNormal
        {
            get { return _backColorNormal; }
        }

        public virtual Color BackColorHover
        {
            get { return _backColorHover; }
        }

        public virtual Color BackColorPressed
        {
            get { return _backColorPressed; }
        }

        public virtual Color ForeColor
        {
            get { return _foreColor; }
        }
    }
}
