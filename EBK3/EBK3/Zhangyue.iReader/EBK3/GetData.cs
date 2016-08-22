namespace Zhangyue.iReader.EBK3
{
    using StructLib;
    using System;
    using System.Collections.Generic;
    using System.Diagnostics;
    using System.IO;
    using System.Runtime.CompilerServices;
    using System.Text;
    using System.Threading.Tasks;
    using Zhangyue.iReader.EBK2;

    public class GetData
    {
        private Ebk3_Data ebk_data = new Ebk3_Data();

        public byte[] decodeingtext(byte[] buff, int key)
        {
            int length = 0x10;
            if (buff.Length < 0x10)
            {
                length = buff.Length;
            }
            byte[] data = new byte[length];
            Array.Copy(buff, 0, data, 0, length);
            byte[] buffer2 = this.ebk_data.pbk_sys_data_decode(data, key);
            Array.Copy(buffer2, 0, buff, 0, buffer2.Length);
            return buff;
        }

        public byte[] encodeingtext(byte[] buff, int key)
        {
            int length = 0x10;
            if (buff.Length < 0x10)
            {
                length = buff.Length;
            }
            byte[] data = new byte[length];
            Array.Copy(buff, 0, data, 0, length);
            byte[] buffer2 = this.ebk_data.pbk_sys_data_encode(data, key);
            Array.Copy(buffer2, 0, buff, 0, buffer2.Length);
            return buff;
        }

        public byte[] GetByte(string filefullname)
        {
            return new byte[0];
        }

        public byte[] GetEbk3HeadData(EBK3HeadInfoModel infomodel, uint time)
        {
            EBK3HeadStruct struct2 = new EBK3HeadStruct();
            new Model();
            GetEbk3HeadArray array = new GetEbk3HeadArray();
            Ebk3_Data data = new Ebk3_Data();
            byte[] buffer = new byte[4];
            Encoding.UTF8.GetBytes("EBK3").CopyTo(buffer, 0);
            struct2.identifier = buffer;
            struct2.header_key = 0;
            struct2.decode_key = time;
            object[] infomodellist = array.infomodelarray(infomodel);
            byte[] recodeByte = this.GetRecodeByte(infomodellist);
            byte[] buffer3 = data.pbk_sys_data_encode(recodeByte, (int)struct2.decode_key);
            struct2.header_length = (uint)(0x10 + recodeByte.Length);
            byte[] buffer4 = new byte[struct2.header_length];
            byte[] buffer5 = Ebk3Make.StrucTObytes(struct2);
            Array.Copy(buffer5, 0, buffer4, 0, buffer5.Length);
            Array.Copy(buffer3, 0, buffer4, buffer5.Length, buffer3.Length);
            return buffer4;
        }

        public byte[] GetPicByte(string filepath)
        {
            return new byte[0];
        }

        public byte[] GetPicdata(string picfilepath)
        {
            return new byte[0];
        }

        public byte[] GetRecodeByte(object[] infomodellist)
        {
            uint length = (uint)infomodellist.Length;
            byte[] bytes = BitConverter.GetBytes(length);
            byte[] buffer2 = new byte[0x100000];
            int num2 = 0;
            Model model = new Model();
            for (int i = 0; i < infomodellist.Length; i++)
            {
                byte[] buffer3 = infomodellist[i] as byte[];
                model.record_type = (uint)(i + 1);
                byte[] buffer4 = new byte[4];
                buffer4 = BitConverter.GetBytes(model.record_type);
                model.record_length = (uint)(8 + buffer3.Length);
                byte[] buffer5 = new byte[4];
                buffer5 = BitConverter.GetBytes(model.record_length);
                byte[] buffer6 = new byte[8 + buffer3.Length];
                Array.Copy(buffer4, 0, buffer6, 0, buffer4.Length);
                Array.Copy(buffer5, 0, buffer6, 4, buffer5.Length);
                Array.Copy(buffer3, 0, buffer6, 8, buffer3.Length);
                Array.Copy(buffer6, 0, buffer2, num2, buffer6.Length);
                num2 += buffer6.Length;
            }
            byte[] buffer7 = new byte[num2 + 4];
            Array.Copy(bytes, 0, buffer7, 0, bytes.Length);
            Array.Copy(buffer2, 0, buffer7, 4, num2);
            return buffer7;
        }

        public byteinfo GetTextChapter(BookInfo bookinfo, int chapterstart, int chapterblock, int key)
        {
            string text = bookinfo.Text;
            List<chapterlist> list = new List<chapterlist>();
            Model model = new Model();
            byteinfo byteinfo = new byteinfo();
            ChapterListModel infomodel = new ChapterListModel();
            GetChapterListArray array = new GetChapterListArray();
            int num = 0;
            int lenth = 0;
            int offset = 0;
            string str2 = text.Replace("\r\n", "\n");
            for (int i = 0; i < bookinfo.ChaterList.Count; i++)
            {
                int index = bookinfo.ChaterList[i].Index;
                int length = 0;
                if (i != (bookinfo.ChaterList.Count - 1))
                {
                    length = bookinfo.ChaterList[i + 1].Index;
                }
                else
                {
                    length = str2.Length;
                }
                string s = str2.Substring(index, length - index).Replace("\n", "\r\n");
                lenth = s.Length * 2;
                string chaptername = bookinfo.ChaterList[i].Name.Replace("\n", "");
                chapterlist chapterlist = new chapterlist(offset, lenth, chaptername);
                list.Add(chapterlist);
                num += Encoding.UTF8.GetBytes(s).Length;
                offset += lenth;
            }
            int num7 = offset;
            int num8 = num7 / model.CATHY_LZ77_WINDOW_SIZE;
            if ((num7 % model.CATHY_LZ77_WINDOW_SIZE) != 0)
            {
                num8++;
            }
            uint num9 = 0;
            uint num10 = 0;
            byte[] buffer = new byte[0xa00000];
            byte[] bytes = Encoding.Unicode.GetBytes(text);
            while (num7 >= model.CATHY_LZ77_WINDOW_SIZE)
            {
                byte[] inputBytes = new byte[0x10000];
                Array.Copy(bytes, num10 * model.CATHY_LZ77_WINDOW_SIZE, inputBytes, 0, inputBytes.Length);
                byte[] buffer4 = this.encodeingtext(DeCompress.Compress(inputBytes), key);
                Array.Copy(buffer4, 0, buffer, (int)num9, buffer4.Length);
                int num11 = (int)num9;
                int num12 = buffer4.Length;
                m_compressList list2 = new m_compressList(num11, num12);
                byteinfo.compressblocksizelist.Add(list2);
                num10++;
                num9 += (uint)(buffer4.Length);
                num7 -= model.CATHY_LZ77_WINDOW_SIZE;
            }
            if (num7 > 0)
            {
                byte[] buffer5 = new byte[num7];
                Array.Copy(bytes, bytes.Length - num7, buffer5, 0, buffer5.Length);
                byte[] buffer6 = this.encodeingtext(DeCompress.Compress(buffer5), key);
                Array.Copy(buffer6, 0, buffer, (int)num9, buffer6.Length);
                int num13 = (int)num9;
                int num14 = buffer6.Length;
                num10++;
                m_compressList list3 = new m_compressList(num13, num14);
                byteinfo.compressblocksizelist.Add(list3);
                num9 += (uint)buffer6.Length;
            }
            byte[] buffer7 = new byte[0x100000];
            int num15 = 0;
            for (int j = 0; j < bookinfo.ChaterList.Count; j++)
            {
                chapterlist chapterlist2 = list[j];
                infomodel.chapter_index1 = (uint)(chapterstart + j);
                infomodel.chapter_level2 = 1;
                infomodel.chapter_data_type3 = 1;
                infomodel.chapter_data_block_offset4 = (uint)chapterblock;
                infomodel.chapter_content_decompresss_offset5 = (uint)chapterlist2._offset;
                infomodel.chapter_content_decompress_length6 = (uint)chapterlist2._lenth;
                infomodel.chapter_name_length7 = (uint)Encoding.Unicode.GetBytes(chapterlist2._chaptername).Length;
                infomodel.chapter_name_data8 = chapterlist2._chaptername;
                byte[] buffer8 = array.infomodelarray(infomodel, key);
                Array.Copy(buffer8, 0, buffer7, num15, buffer8.Length);
                num15 += buffer8.Length;
            }
            byte[] buffer9 = new byte[num9];
            byte[] buffer10 = new byte[num15];
            Array.Copy(buffer7, 0, buffer10, 0, buffer10.Length);
            Array.Copy(buffer, 0, buffer9, 0, buffer9.Length);
            byteinfo.compressysdate = buffer9;
            byteinfo.chaptertxtbyteresult = buffer10;
            return byteinfo;
        }

        public byte[] OnlyChapter(string html, int allheadenth, string htmlname, int index, int encodeing, int key)
        {
            GetChapterListArray array = new GetChapterListArray();
            ChapterListModel infomodel = new ChapterListModel
            {
                chapter_index1 = (uint)index,
                chapter_level2 = 1,
                chapter_data_type3 = 0,
                chapter_data_block_offset4 = (uint)allheadenth,
                chapter_content_decompresss_offset5 = 0
            };
            if (encodeing == 3)
            {
                infomodel.chapter_content_decompress_length6 = (uint)(Encoding.UTF8.GetBytes(html).Length);
            }
            if (encodeing == 1)
            {
                infomodel.chapter_content_decompress_length6 = (uint)(Encoding.Unicode.GetBytes(html).Length);
            }
            infomodel.chapter_name_length7 = (uint)(Encoding.Unicode.GetBytes(htmlname).Length);
            infomodel.chapter_name_data8 = htmlname;
            return array.infomodelarray(infomodel, key);
        }

        public byte[] ReturnBlockRecord(blockinfoModel blockinfomodel, int count, int key)
        {
            object[] infomodellist = new blockinfoArray().infoblockinfoarray(blockinfomodel, count);
            byte[] recodeByte = this.GetRecodeByte(infomodellist);
            return this.ebk_data.pbk_sys_data_encode(recodeByte, key);
        }
    }
}

