using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace tisn.cdr
{
    public class RiffConn
    {
        public static List<RIFFBase> ReadRiff(CDRFile p, BinaryReader br, int l = -1)
        {
            return ReadRiff(p, br, new int[0], l);
        }
        public static List<RIFFBase> ReadRiff(CDRFile p, BinaryReader br, int[] blockSizes, int l = -1)
        {
            int fcc;
            int size;
            long len = l == -1 ? br.BaseStream.Length : br.BaseStream.Position + l;
            List<RIFFBase> list = new List<RIFFBase>();
            while (br.BaseStream.Position < len)
            {
                fcc = br.ReadInt32();
                size = br.ReadInt32();
                if (blockSizes != null && blockSizes.Length > 0)
                {
                    size = blockSizes[size];
                }
                string type = RiffParser.FromFourCC(fcc);
                if (0 == String.Compare(type, RiffParser.LIST4CC))
                {
                    fcc = br.ReadInt32();
                    String name = RiffParser.FromFourCC(fcc);
                    RIFFBase rl;
                    if (name == "cmpr")
                    {
                        rl = new CDRcmpr(p);
                    }
                    else if (name == "bmkt")
                    {
                        rl = new CDRbmkt(p);
                    }
                    else if (name == "colo")
                    {
                        rl = new CDRcolo(p);
                    }
                    else if (name == "INFO")
                    {
                        rl = new CDRINFO(p);
                    }
                    else if (name == "syst")
                    {
                        rl = new CDRsyst(p);
                    }
                    else if (name == "page")
                    {
                        rl = new CDRpage(p);
                    }
                    else if (name == "gobj")
                    {
                        rl = new CDRgobj(p);
                    }
                    else if (name == "bmpt")
                    {
                        rl = new CDRbmpt(p);
                    }
                    else if (name == "iccp")
                    {
                        rl = new CDRiccp(p);
                    }
                    else if (name == "filt")
                    {
                        rl = new CDRfilt(p);
                    }
                    else if (name == "filc")
                    {
                        rl = new CDRfilc(p);
                    }
                    else if (name == "otlt")
                    {
                        rl = new CDRotlt(p);
                    }
                    else if (name == "stlt")
                    {
                        rl = new CDRstlt(p);
                    }
                    else if (name == "uil ")
                    {
                        rl = new CDRuil_(p);
                    }
                    else if (name == "doc ")
                    {
                        rl = new CDRdoc_(p);
                    }
                    else if (name == "layr")
                    {
                        rl = new CDRlayr(p);
                    }
                    else if (name == "lgob")
                    {
                        rl = new CDRlgob(p);
                    }
                    else if (name == "fntt")
                    {
                        rl = new CDRfntt(p);
                    }
                    else if (name == "grp ")
                    {
                        rl = new CDRgrp_(p);
                    }
                    else if (name == "obj ")
                    {
                        rl = new CDRobj_(p);
                    }
                    else
                    {
                        rl = new RiffList(p);
                    }
                    rl.name = name;
                    rl.Read(br, size - 4, blockSizes);
                    if (0 != (size & 1))
                    {
                        br.BaseStream.Position++;
                    }
                    list.Add(rl);
                }
                else
                {
                    RIFFBase rl;
                    String name = RiffParser.FromFourCC(fcc);
                    if (name == "vrsn")
                    {
                        rl = new CDRvrsn(p);
                    }
                    else if (name == "sumi")
                    {
                        rl = new CDRsumi(p);
                    }
                    else if (name == "mcfg")
                    {
                        rl = new CDRmcfg(p);
                    }
                    else if (name == "fver")
                    {
                        rl = new CDRfver(p);
                    }
                    else if (name == "DISP")
                    {
                        rl = new CDRDISP(p);
                    }
                    else if (name == "iccd")
                    {
                        rl = new CDRiccd(p);
                    }
                    else if (name == "IKEY")
                    {
                        rl = new CDRIKEY(p);
                    }
                    else if (name == "ICMT")
                    {
                        rl = new CDRICMT(p);
                    }
                    else if (name == "pref")
                    {
                        rl = new CDRpref(p);
                    }
                    else if (name == "ptrt")
                    {
                        rl = new CDRptrt(p);
                    }
                    else if (name == "fild")
                    {
                        rl = new CDRfild(p);
                    }
                    else if (name == "outl")
                    {
                        rl = new CDRoutl(p);
                    }
                    else if (name == "flgs")
                    {
                        rl = new CDRflgs(p);
                    }
                    else if (name == "bbox")
                    {
                        rl = new CDRbbox(p);
                    }
                    else if (name == "loda")
                    {
                        rl = new CDRloda(p);
                    }
                    else if (name == "bmp ")
                    {
                        rl = new CDRbmp_(p);
                    }
                    else if (name == "bmrk")
                    {
                        rl = new CDRbmrk(p);
                    }
                    else if (name == "font")
                    {
                        rl = new CDRfont(p);
                    }
                    else
                    {
                        rl = new RiffChunk(p);
                    }
                    rl.name = name;
                    rl.Read(br, size);
                    if (0 != (size & 1))
                    {
                        br.BaseStream.Position++;
                    }
                    list.Add(rl);
                }
            }
            return list;
        }
        public static byte[] WriteRiff(List<RIFFBase> list)
        {
            int[] t = new int[0];
            return WriteRiff(list, ref t, false);
        }
        public static byte[] WriteRiff(List<RIFFBase> list, ref int[] blockSizes, bool outBlockSize = false)
        {
            byte[] tmp = new byte[0xffffff];
            List<int> bs = null;
            if (outBlockSize)
            {
                bs = new List<int>(blockSizes);
            }
            MemoryStream ms = new MemoryStream();
            BinaryWriter bw = new BinaryWriter(ms);
            foreach (RIFFBase rl in list)
            {
                switch (rl.Type)
                {
                    case RiffType.Chunk:
                        bw.Write(RiffParser.ToFourCC(rl.name));
                        byte[] content = ((RiffChunk)rl).GetContent();
                        if (bs != null)
                        {
                            bs.Add(content.Length);
                            bw.Write(bs.Count - 1);
                        }
                        else
                        {
                            bw.Write(content.Length);
                        }
                        bw.Write(content);
                        if ((content.Length & 1) != 0)
                        {
                            bw.Write(false);
                        }
                        break;
                    case RiffType.LIST:
                        bw.Write(RiffParser.ToFourCC("LIST"));
                        int bsIndex = -1;
                        if (bw != null)
                        {
                            bsIndex = bs.Count;
                            bs.Add(0);
                        }
                        bw.Write(0);
                        bw.Write(RiffParser.ToFourCC(rl.name));
                        long op = bw.BaseStream.Position;
                        blockSizes = bs.ToArray();
                        if (((RiffList)rl).subList.Count > 0)
                        {
                            byte[] t1 = WriteRiff(((RiffList)rl).subList, ref blockSizes, outBlockSize);
                            bs = new List<int>(blockSizes);
                            bw.Write(t1);
                        }
                        else
                        {
                            bw.Write(rl.content);
                        }
                        long sp = bw.BaseStream.Position;
                        int len = (int)(sp - op);
                        bw.BaseStream.Position -= len + 8;
                        if (bs == null)
                        {
                            bw.Write(len);
                        }
                        else
                        {
                            bs.Insert(bsIndex, len + 4);
                            bs.RemoveAt(bsIndex + 1);
                            bw.Write(bsIndex);
                        }
                        bw.BaseStream.Position = sp;
                        break;
                }
            }
            int p = (int)ms.Position;
            byte[] o = new byte[p];
            Array.Copy(ms.GetBuffer(), o, p);
            bw.Close();
            ms.Close();
            blockSizes = bs.ToArray();
            return o;
        }

        public static List<RIFFBase> DelRiff<T>(List<RIFFBase> list)
        {
            foreach (RIFFBase rb in list)
            {
                if (rb is T)
                {
                    list.Remove(rb);
                    break;
                }
                else if (rb is RiffList)
                {
                    ((RiffList)rb).subList = DelRiff<T>(((RiffList)rb).subList);
                }
            }
            return list;
        }
    }
}
