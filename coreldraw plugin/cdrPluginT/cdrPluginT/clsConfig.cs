using Corel.Interop.VGCore;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Application = Corel.Interop.VGCore.Application;

namespace cdrPluginT
{
    public class clsConfig
    {
        public enum EScope
        {
            eDocuments,
            eDocument,
            ePage,
            eLayer,
            eSelection
        }
        public Color clsFind, clrReplace;
        public bool PreserveTints, ReplaceFills, ReplaceOutlines, ReplaceInsideText;
        public EScope Scope;
        public bool AutoClose, choosing, fNoColor, rNoColor;
        public int cols;
        public bool ShowSpotPalettes;

        private Application mApp;

        public clsConfig(Application app)
        {
            clsFind = new Color();
            clrReplace = new Color();
            clsFind.CMYKAssign(0, 0, 0, 100);
            clrReplace.CMYKAssign(0, 0, 0, 100);
            PreserveTints = false;
            ReplaceFills = true;
            ReplaceOutlines = true;
            ReplaceInsideText = false;
            cols = 16;

            this.mApp = app;
        }

        public void Save()
        {
        }

        public void Load()
        {
            String sDefColor = "138A0000000000000064000000000000";
            String s;
            long nScope;

        }

        private String EncodeColor(Color C)
        {
            int md, c1, c2, c3, c4, c5, c6, c7;
            C.CorelScriptGetComponent(out md, out c1, out c2, out c3, out c4, out c5, out c6, out c7);
            return "";
        }

        private void DecodeColor(ref Color C, String s)
        {
            int md, c1, c2, c3, c4, c5, c6, c7;
            md = HexToDec(s.Substring(1, 4));
            if (mApp.VersionMajor < 11 && (md % 1000) > 30)
            {
                C.CMYKAssign(0, 0, 0, 100);
            }
            else
            {
                c1 = HexToDec(s.Substring(5, 4));
                c2 = HexToDec(s.Substring(9, 4));
                c3 = HexToDec(s.Substring(13, 4));
                c4 = HexToDec(s.Substring(17, 4));
                c5 = HexToDec(s.Substring(21, 4));
                c6 = HexToDec(s.Substring(25, 4));
                c7 = HexToDec(s.Substring(29, 4));
                C.CorelScriptAssign(md, c1, c2, c3, c4, c5, c6, c7);
            }
        }

        private int HexToDec(String x)
        {
            if (string.IsNullOrEmpty(x))
             {
                 return 0;
             }
             string z = null;
             Stack a = new Stack();
             int i = 0, j = 0, l = x.Length;
             long Tong = 0;
             while (i < l)
            {
                 a.Push(ToDec(Convert.ToString(x[i])));
                i++;
             }
             while (a.Count != 0)
             {
                 Tong = Tong + Convert.ToInt64(a.Pop())*Pow(16, j);
                 j++;
             }
             return (int)Tong;
        }
        private static long Pow(long x, long y)
        {
            int i = 1;
            long X = x;
            if (y == 0)
                return 1;
            while (i < y)
            {
                x = x * X;
                i++;
            }
            return x;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="x"></param>
        /// <returns></returns>
        private static string ToDec(string x)
        {
            switch (x)
            {
                case "A":
                    return "10";
                case "B":
                    return "11";
                case "C":
                    return "12";
                case "D":
                    return "13";
                case "E":
                    return "14";
                case "F":
                    return "15";
                default:
                    return x;
            }
        }
        public static string DecToHex(int X)
        {
            string z = null;
            Stack a = new Stack();
            int i = 0;
            while (X > 0)
            {
                a.Push(Convert.ToString(X % 16));
                X = X / 16;
                i++;
            }
            while (a.Count != 0)
                z += ToHex(Convert.ToString(a.Pop()));
            if (string.IsNullOrEmpty(z))
            {
                z = "0";
            }
            return z;
        }

        private static string ToHex(string x)
        {
            switch (x)
            {
                case "10":
                    return "A";
                case "11":
                    return "B";
                case "12":
                    return "C";
                case "13":
                    return "D";
                case "14":
                    return "E";
                case "15":
                    return "F";
                default:
                    return x;
            }
        }
    }
}
