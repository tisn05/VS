using System;
using System.Collections.Generic;
using System.Text;

namespace tisn.cdr
{
    public class CDRFile
    {
        public String verType;
        public short Version = 0;
        public int SVersion = 0;
        public int BuildID = 0;
        /// <summary>
        /// 位图数量
        /// </summary>
        public int BMPCount = 0;
        /// <summary>
        /// 视图样式数量
        /// </summary>
        public int ViewCount = 0;

        /// <summary>
        /// 对象数
        /// </summary>
        public int ObjectCount = 0;

        /// <summary>
        /// 矩形数量
        /// </summary>
        public int Rectangles = 0;

        /// <summary>
        /// 椭圆数量
        /// </summary>
        public int Ellipses = 0;

        /// <summary>
        /// 曲线数量
        /// </summary>
        public int Curves = 0;

        /// <summary>
        /// 美术字数量
        /// </summary>
        public int ArtisticTexts = 0;

        /// <summary>
        /// 段落文本数量
        /// </summary>
        public int ParagraphText = 0;

        /// <summary>
        /// 多边形数量
        /// </summary>
        public int Polygons = 0;

        /// <summary>
        /// 群组数
        /// </summary>
        public int GroupCount = 0;

        /// <summary>
        /// 页面数
        /// </summary>
        public int PageCount = 0;

        /// <summary>
        /// 填充数量
        /// </summary>
        public int FillCout = 0;

        /// <summary>
        /// 轮廓数量
        /// </summary>
        public int OutLineCount = 0;

        /// <summary>
        /// 字体编码
        /// </summary>
        public static Dictionary<int, String> Font_Charsets = new Dictionary<int, string>();

        /// <summary>
        /// 字体列表
        /// </summary>
        public List<Fonts> fonts = new List<Fonts>();

        /// <summary>
        /// 填充类型
        /// </summary>
        public static Dictionary<uint, String> Fill_Type = new Dictionary<uint, string>();

        public class Fonts
        {
            public String name = null;
            public String encoding = null;
        }

        public List<RIFFBase> list;

        public int PageWidth = 0;
        public int PageHeight = 0;

        public CDRFile()
        {
            if (Font_Charsets.Count == 0)
            {
                Font_Charsets.Add(0, "拉丁文");
                Font_Charsets.Add(1, "系统默认");
                Font_Charsets.Add(2, "符号");
                Font_Charsets.Add(77, "苹果罗马");
                Font_Charsets.Add(128, "日文Shift-JIS");
                Font_Charsets.Add(129, "韩文(Hangul)");
                Font_Charsets.Add(130, "韩文(Johab)");
                Font_Charsets.Add(134, "简体中文GBK");
                Font_Charsets.Add(136, "繁体中文BIG5");
                Font_Charsets.Add(161, "希腊语");
                Font_Charsets.Add(162, "土耳其语");
                Font_Charsets.Add(163, "越南语");
                Font_Charsets.Add(177, "希伯来语");
                Font_Charsets.Add(178, "阿拉伯语");
                Font_Charsets.Add(186, "波罗的语");
                Font_Charsets.Add(204, "斯拉夫语");
                Font_Charsets.Add(222, "泰国语");
                Font_Charsets.Add(238, "Latin II (Central European)");
                Font_Charsets.Add(255, "OEM Latin I");
            }

            if (Fill_Type.Count == 0)
            {
                Fill_Type.Add(0, "无填充");
                Fill_Type.Add(1, "标准");
                Fill_Type.Add(2, "渐变");
                Fill_Type.Add(6, "Postscript");
                Fill_Type.Add(7, "Pattern");
                Fill_Type.Add(9, "位图");
                Fill_Type.Add(10, "Fullcolour");
                Fill_Type.Add(11, "Texture");
            }
        }

        public static String GetVersionName(String verType)
        {
            String n = verType.Substring(3, 1);
            String rs = "";
            if (n == "")
                rs = "";
            if (n == "7")
                rs = "7";
            else if (n == "8")
                rs = "8";
            else if (n == "9")
                rs = "9";
            else if (n == "A")
                rs = "10";
            else if (n == "B")
                rs = "X1(11)";
            else if (n == "C")
                rs = "X2(12)";
            else if (n == "D")
                rs = "X3(13)";
            else if (n == "E")
                rs = "X4(14)";
            else if (n == "F")
                rs = "X5(15)";
            else if (n == "G")
                rs = "X6(16)";
            else if (n == "H")
                rs = "X7(17)";
            return rs;
        }
    }
}
