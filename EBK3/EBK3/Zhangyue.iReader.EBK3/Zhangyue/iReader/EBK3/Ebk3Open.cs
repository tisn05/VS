using StructLib;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
namespace Zhangyue.iReader.EBK3
{
    public class Ebk3Open
    {
        private Ebk3_Data ebk3_data = new Ebk3_Data();
        private List<ChapterListModel> mChapterList;
        private byte[] mEBK3Buffer;
        private Stream mEBK3Stream;

        public void EBK3Close()
        {
            if (this.mEBK3Stream != null)
            {
                this.mEBK3Stream.Dispose();
                this.mEBK3Stream = null;
            }
        }

        public string EBK3GetChapter(int mChapterIndex)
        {
            if (this.mEBK3Stream == null)
            {
                return null;
            }
            byte[] buffer = this.mEBK3Buffer;
            new BookInfo();
            string str = "";
            if ((mChapterIndex < 0) || (mChapterIndex >= this.mChapterList.Count))
            {
                return str;
            }
            ChapterListModel model = this.mChapterList[mChapterIndex];
            if (model.chapter_data_type3 == 0)
            {
                byte[] buffer2 = new byte[buffer.Length - model.chapter_data_block_offset4];
                Array.Copy(buffer, (int)model.chapter_data_block_offset4, buffer2, 0, buffer2.Length);
                return this.Getchapter(buffer2, 0);
            }
            byte[] blockdata = new byte[buffer.Length - model.chapter_data_block_offset4];
            Array.Copy(buffer, (int)model.chapter_data_block_offset4, blockdata, 0, blockdata.Length);
            return this.GetOneChapter(blockdata, 1, mChapterIndex);
        }

        public List<ChapterListModel> EBK3GetChapterList()
        {
            if (this.mEBK3Stream == null)
            {
                return null;
            }
            return this.mChapterList;
        }

        private List<int> getblocksize(byte[] buffer)
        {
            List<int> list = new List<int>();
            byte[] buffer2 = new byte[4];
            Array.Copy(buffer, 0, buffer2, 0, 4);
            int num = (int)BitConverter.ToUInt32(buffer2, 0);
            int num2 = 4;
            for (int i = 0; i < num; i++)
            {
                byte[] buffer3 = new byte[4];
                Array.Copy(buffer, num2, buffer3, 0, buffer3.Length);
                int num4 = (int)BitConverter.ToUInt32(buffer3, 0);
                num2 += buffer3.Length;
                list.Add(num4);
            }
            return list;
        }

        private string Getchapter(byte[] blockdata, int type)
        {
            blockStruct blockStruct = new blockStruct();
            GetData data = new GetData();
            blockStruct = new ByteToStructEbk3().GetBlockStruct(blockdata);
            byte[] buffer = new byte[blockStruct.header_length - 12];
            Array.Copy(blockdata, 12, buffer, 0, buffer.Length);
            blockinfoModel recodeBlockModel = this.GetRecodeBlockModel(this.ebk3_data.pbk_sys_data_decode(buffer, (int)blockStruct.decode_key), 11);
            if (recodeBlockModel.file_list_offset11 != 0)
            {
                byte[] buffer2 = new byte[blockdata.Length - recodeBlockModel.file_list_offset11];
                Array.Copy(blockdata, (int)recodeBlockModel.file_list_offset11, buffer2, 0, buffer2.Length);
            }
            byte[] buffer3 = new byte[4];
            Array.Copy(blockdata, (int)blockStruct.header_length, buffer3, 0, 4);
            int num = (int)BitConverter.ToUInt32(this.ebk3_data.pbk_sys_data_decode(buffer3, (int)blockStruct.decode_key), 0);
            int num2 = (int)(blockStruct.header_length + 4);
            byte[] buffer4 = new byte[0x1400000];
            int num3 = 0;
            int num4 = 0;
            byte[] buffer5 = new byte[num * 4];
            Array.Copy(blockdata, num2, buffer5, 0, buffer5.Length);
            byte[] buffer6 = this.ebk3_data.pbk_sys_data_decode(buffer5, (int)blockStruct.decode_key);
            for (int i = 0; i < num; i++)
            {
                byte[] buffer7 = new byte[4];
                Array.Copy(buffer6, i * 4, buffer7, 0, 4);
                int num6 = (int)BitConverter.ToUInt32(buffer7, 0);
                byte[] buff = new byte[num6];
                Array.Copy(blockdata, (num2 + (num * 4)) + num4, buff, 0, buff.Length);
                byte[] buffer9 = new byte[num6];
                byte[] buffer10 = DeCompress.Decompress(data.decodeingtext(buff, (int)blockStruct.decode_key));
                Array.Copy(buffer10, 0, buffer4, num3, buffer10.Length);
                num4 += num6;
                num3 += buffer10.Length;
            }
            byte[] bytes = new byte[num3];
            Array.Copy(buffer4, 0, bytes, 0, bytes.Length);
            if (type == 0)
            {
                return Encoding.Unicode.GetString(bytes, 0, bytes.Length);
            }
            return Encoding.Unicode.GetString(bytes, 0, bytes.Length);
        }

        private List<ChapterListModel> GetChapterList(int chaptercount, int headlenth, byte[] buff, int key)
        {
            List<ChapterListModel> list = new List<ChapterListModel>();
            ChapterListStruct blockFileList = new ChapterListStruct();
            int num = 0;
            for (int i = 0; i < chaptercount; i++)
            {
                ChapterListModel model = new ChapterListModel();
                byte[] buffs = new byte[0x18];
                Array.Copy(buff, num, buffs, 0, buffs.Length);
                byte[] data = new byte[4];
                Array.Copy(buffs, 12, data, 0, 4);
                Array.Copy(this.ebk3_data.pbk_sys_data_decode(data, key), 0, buffs, 12, 4);
                blockFileList = new ByteToStructEbk3().GetBlockFileList(buffs);
                model.chapter_index1 = blockFileList.chapter_index1;
                model.chapter_level2 = blockFileList.chapter_level2;
                model.chapter_data_type3 = blockFileList.chapter_data_type3;
                model.chapter_data_block_offset4 = blockFileList.chapter_data_block_offset4;
                model.chapter_content_decompresss_offset5 = blockFileList.chapter_content_decompresss_offset5;
                model.chapter_content_decompress_length6 = blockFileList.chapter_content_decompress_length6;
                model.chapter_name_length7 = blockFileList.chapter_name_length7;
                byte[] buffer3 = new byte[model.chapter_name_length7];
                Array.Copy(buff, num + buffs.Length, buffer3, 0, buffer3.Length);
                model.chapter_name_data8 = Encoding.Unicode.GetString(this.ebk3_data.pbk_sys_data_decode(buffer3, key), 0, buffer3.Length);
                num += buffs.Length + buffer3.Length;
                list.Add(model);
            }
            return list;
        }

        private byte[] getHeadDy(DynDataStruct dy)
        {
            byte[] bytes = BitConverter.GetBytes(dy.dyn_data_length);
            byte[] buffer2 = BitConverter.GetBytes(dy.book_id);
            byte[] buffer3 = BitConverter.GetBytes(dy.chapter_list_offset);
            byte[] buffer4 = BitConverter.GetBytes(dy.chapter_list_length);
            byte[] buffer5 = new byte[0x10];
            Array.Copy(bytes, 0, buffer5, 0, 4);
            Array.Copy(buffer2, 0, buffer5, 4, 4);
            Array.Copy(buffer3, 0, buffer5, 8, 4);
            Array.Copy(buffer4, 0, buffer5, 12, 4);
            return buffer5;
        }

        private byte[] getHeadDy(byte[] head, DynDataStruct dy)
        {
            byte[] bytes = BitConverter.GetBytes(dy.dyn_data_length);
            byte[] buffer2 = BitConverter.GetBytes(dy.book_id);
            byte[] buffer3 = BitConverter.GetBytes(dy.chapter_list_offset);
            byte[] buffer4 = BitConverter.GetBytes(dy.chapter_list_length);
            byte[] buffer5 = new byte[0x10 + head.Length];
            Array.Copy(head, 0, buffer5, 0, head.Length);
            Array.Copy(bytes, 0, buffer5, head.Length, 4);
            Array.Copy(buffer2, 0, buffer5, head.Length + 4, 4);
            Array.Copy(buffer3, 0, buffer5, head.Length + 8, 4);
            Array.Copy(buffer4, 0, buffer5, head.Length + 12, 4);
            return buffer5;
        }

        private string GetOneChapter(byte[] blockdata, int type, int mChapterIndex)
        {
            blockStruct blockStruct = new blockStruct();
            GetData data = new GetData();
            blockStruct = new ByteToStructEbk3().GetBlockStruct(blockdata);
            byte[] buffer = new byte[blockStruct.header_length - 12];
            Array.Copy(blockdata, 12, buffer, 0, buffer.Length);
            blockinfoModel recodeBlockModel = this.GetRecodeBlockModel(this.ebk3_data.pbk_sys_data_decode(buffer, (int)blockStruct.decode_key), 13);
            if (recodeBlockModel.file_list_offset11 != 0)
            {
                byte[] buffer2 = new byte[blockdata.Length - recodeBlockModel.file_list_offset11];
                Array.Copy(blockdata, (int)recodeBlockModel.file_list_offset11, buffer2, 0, buffer2.Length);
            }
            byte[] buffer3 = new byte[4];
            Array.Copy(blockdata, (int)blockStruct.header_length, buffer3, 0, 4);
            int num = (int)BitConverter.ToUInt32(this.ebk3_data.pbk_sys_data_decode(buffer3, (int)blockStruct.decode_key), 0);
            int num2 = (int)(blockStruct.header_length + 4);
            byte[] buffer4 = new byte[num * 4];
            Array.Copy(blockdata, num2, buffer4, 0, buffer4.Length);
            byte[] buffer5 = this.ebk3_data.pbk_sys_data_decode(buffer4, (int)blockStruct.decode_key);
            uint num3 = this.mChapterList[mChapterIndex].chapter_content_decompresss_offset5 / 0x10000;
            if (num3 > num)
            {
                return "";
            }
            int num4 = 0;
            int num5 = (int)(this.mChapterList[mChapterIndex].chapter_content_decompresss_offset5 % 0x10000);
            int num6 = 0;
            for (int i = 0; i < num3; i++)
            {
                byte[] buffer6 = new byte[4];
                Array.Copy(buffer5, i * 4, buffer6, 0, 4);
                num6 = (int)BitConverter.ToUInt32(buffer6, 0);
                num4 += num6;
            }
            byte[] buffer7 = new byte[4];
            Array.Copy(buffer5, (int)(num3 * 4), buffer7, 0, 4);
            num6 = (int)BitConverter.ToUInt32(buffer7, 0);
            byte[] buff = new byte[num6];
            Array.Copy(blockdata, (num2 + (num * 4)) + num4, buff, 0, buff.Length);
            byte[] buffer9 = new byte[num6];
            byte[] buffer10 = DeCompress.Decompress(data.decodeingtext(buff, (int)blockStruct.decode_key));
            byte[] bytes = new byte[this.mChapterList[mChapterIndex].chapter_content_decompress_length6];
            string str = "";
            if ((num5 + bytes.Length) < 0x10000)
            {
                Array.Copy(buffer10, num5, bytes, 0, bytes.Length);
            }
            else
            {
                Array.Copy(buffer10, num5, bytes, 0, 0x10000 - num5);
            }
            if ((num5 + this.mChapterList[mChapterIndex].chapter_content_decompress_length6) > 0x10000L)
            {
                num3++;
                if (num3 >= num)
                {
                    return "";
                }
                int num1 = ((int)(num5 + this.mChapterList[mChapterIndex].chapter_content_decompress_length6)) % 0x10000;
                num6 = 0;
                num4 = 0;
                for (int j = 0; j < num3; j++)
                {
                    byte[] buffer12 = new byte[4];
                    Array.Copy(buffer5, j * 4, buffer12, 0, 4);
                    num6 = (int)BitConverter.ToUInt32(buffer12, 0);
                    num4 += num6;
                }
                buffer7 = new byte[4];
                Array.Copy(buffer5, (int)(num3 * 4), buffer7, 0, 4);
                num6 = (int)BitConverter.ToUInt32(buffer7, 0);
                buff = new byte[num6];
                Array.Copy(blockdata, (num2 + (num * 4)) + num4, buff, 0, buff.Length);
                buffer9 = new byte[num6];
                Array.Copy(DeCompress.Decompress(data.decodeingtext(buff, (int)blockStruct.decode_key)), 0, bytes, 0x10000 - num5, bytes.Length - (0x10000 - num5));
            }
            if (type != 0)
            {
                str = Encoding.Unicode.GetString(bytes, 0, bytes.Length);
            }
            return str;
        }

        private byte[] getOneChapterList(ChapterListModel OneChapter, int key)
        {
            int num = 0;
            byte[] bytes = BitConverter.GetBytes(OneChapter.chapter_index1);
            byte[] buffer2 = BitConverter.GetBytes(OneChapter.chapter_level2);
            byte[] buffer3 = BitConverter.GetBytes(OneChapter.chapter_data_type3);
            byte[] buffer4 = this.ebk3_data.pbk_sys_data_encode(BitConverter.GetBytes(OneChapter.chapter_data_block_offset4), key);
            byte[] buffer5 = BitConverter.GetBytes(OneChapter.chapter_content_decompresss_offset5);
            byte[] buffer6 = BitConverter.GetBytes(OneChapter.chapter_content_decompress_length6);
            byte[] buffer7 = this.ebk3_data.pbk_sys_data_encode(Encoding.Unicode.GetBytes(OneChapter.chapter_name_data8), key);
            byte[] buffer8 = BitConverter.GetBytes((int)buffer7.Length);
            byte[] buffer9 = new byte[((((((buffer8.Length + bytes.Length) + buffer2.Length) + buffer3.Length) + buffer4.Length) + buffer5.Length) + buffer6.Length) + buffer7.Length];
            Array.Copy(buffer8, 0, buffer9, 0, buffer8.Length);
            num += buffer8.Length;
            Array.Copy(bytes, 0, buffer9, num, bytes.Length);
            num += bytes.Length;
            Array.Copy(buffer2, 0, buffer9, num, buffer2.Length);
            num += buffer2.Length;
            Array.Copy(buffer3, 0, buffer9, num, buffer3.Length);
            num += buffer3.Length;
            Array.Copy(buffer4, 0, buffer9, num, buffer4.Length);
            num += buffer4.Length;
            Array.Copy(buffer5, 0, buffer9, num, buffer5.Length);
            num += buffer5.Length;
            Array.Copy(buffer6, 0, buffer9, num, buffer6.Length);
            num += buffer6.Length;
            Array.Copy(buffer7, 0, buffer9, num, buffer7.Length);
            return buffer9;
        }

        private blockinfoModel GetRecodeBlockModel(byte[] allbuffer, int count)
        {
            blockinfoModel model = new blockinfoModel();
            byte[] buffer = new byte[allbuffer.Length - 4];
            byte[] buffer2 = new byte[4];
            Array.Copy(allbuffer, 0, buffer2, 0, 4);
            BitConverter.ToUInt32(buffer2, 0);
            Array.Copy(allbuffer, 4, buffer, 0, buffer.Length);
            int num = 0;
            for (int i = 0; i < count; i++)
            {
                byte[] buffer3 = new byte[4];
                byte[] buffer4 = new byte[4];
                Array.Copy(buffer, num, buffer3, 0, buffer3.Length);
                uint num3 = BitConverter.ToUInt32(buffer3, 0);
                Array.Copy(buffer, num + 4, buffer4, 0, buffer4.Length);
                uint num4 = BitConverter.ToUInt32(buffer4, 0);
                byte[] buffer5 = new byte[num4 - 8];
                Array.Copy(buffer, num + 8, buffer5, 0, buffer5.Length);
                num += (int)num4;
                switch (num3)
                {
                    case 1:
                        model.min_version1 = BitConverter.ToUInt32(buffer5, 0);
                        break;

                    case 2:
                        model.min_tip2 = Encoding.Unicode.GetString(buffer5, 0, buffer5.Length);
                        break;

                    case 3:
                        model.compress_type3 = BitConverter.ToUInt32(buffer5, 0);
                        break;

                    case 4:
                        model.compress_block_size4 = BitConverter.ToUInt32(buffer5, 0);
                        break;

                    case 5:
                        model.datatype5 = BitConverter.ToUInt32(buffer5, 0);
                        break;

                    case 6:
                        model.locale6 = BitConverter.ToUInt32(buffer5, 0);
                        break;

                    case 7:
                        model.encoding7 = BitConverter.ToUInt32(buffer5, 0);
                        break;

                    case 8:
                        model.chapter_index8 = BitConverter.ToUInt32(buffer5, 0);
                        break;

                    case 9:
                        model.chapter_level9 = BitConverter.ToUInt16(buffer5, 0);
                        break;

                    case 10:
                        model.chapter_name10 = Encoding.Unicode.GetString(buffer5, 0, buffer5.Length);
                        break;

                    case 11:
                        model.file_list_offset11 = BitConverter.ToUInt32(buffer5, 0);
                        break;

                    case 12:
                        model.chapter_content_decompress_length12 = BitConverter.ToUInt32(buffer5, 0);
                        break;

                    case 13:
                        model.chapter_file_size13 = BitConverter.ToUInt32(buffer5, 0);
                        break;
                }
            }
            return model;
        }

        public static EBK3HeadInfoModel GetRecodeModel(byte[] allbuffer, int count)
        {
            byte[] buffer = new byte[allbuffer.Length - 4];
            byte[] buffer2 = new byte[4];
            Array.Copy(allbuffer, 0, buffer2, 0, 4);
            BitConverter.ToUInt32(buffer2, 0);
            Array.Copy(allbuffer, 4, buffer, 0, buffer.Length);
            EBK3HeadInfoModel model = new EBK3HeadInfoModel();
            int num = 0;
            for (int i = 0; i < count; i++)
            {
                byte[] buffer3 = new byte[4];
                byte[] buffer4 = new byte[4];
                Array.Copy(buffer, num, buffer3, 0, buffer3.Length);
                uint num3 = BitConverter.ToUInt32(buffer3, 0);
                Array.Copy(buffer, num + 4, buffer4, 0, buffer4.Length);
                uint num4 = BitConverter.ToUInt32(buffer4, 0);
                byte[] buffer5 = new byte[num4 - 8];
                Array.Copy(buffer, num + 8, buffer5, 0, buffer5.Length);
                num += (int)num4;
                switch (num3)
                {
                    case 1:
                        model.min_version1 = BitConverter.ToUInt32(buffer5, 0);
                        break;

                    case 2:
                        model.unique_identifier2 = Encoding.Unicode.GetString(buffer5, 0, buffer5.Length);
                        break;

                    case 3:
                        model.book_file_type3 = BitConverter.ToUInt32(buffer5, 0);
                        break;

                    case 4:
                        model.chapter_list_offset4 = BitConverter.ToUInt32(buffer5, 0);
                        break;

                    case 5:
                        model.chapter_dyn_info_offset5 = BitConverter.ToUInt32(buffer5, 0);
                        break;

                    case 6:
                        model.cover_data_offset6 = BitConverter.ToUInt32(buffer5, 0);
                        break;

                    case 7:
                        model.creator7 = Encoding.Unicode.GetString(buffer5, 0, buffer5.Length);
                        break;

                    case 8:
                        model.create_timer8 = Encoding.Unicode.GetString(buffer5, 0, buffer5.Length);
                        break;

                    case 9:
                        model.name9 = Encoding.Unicode.GetString(buffer5, 0, buffer5.Length);
                        break;

                    case 10:
                        model.author10 = Encoding.Unicode.GetString(buffer5, 0, buffer5.Length);
                        break;

                    case 11:
                        model.subject11 = Encoding.Unicode.GetString(buffer5, 0, buffer5.Length);
                        break;

                    case 12:
                        model.publisher12 = Encoding.Unicode.GetString(buffer5, 0, buffer5.Length);
                        break;

                    case 13:
                        model.isbn13 = Encoding.Unicode.GetString(buffer5, 0, buffer5.Length);
                        break;

                    case 14:
                        model.file_size14 = BitConverter.ToUInt32(buffer5, 0);
                        break;

                    case 15:
                        model.revise_version16 = BitConverter.ToUInt32(buffer5, 0);
                        break;
                }
            }
            return model;
        }

        public BookInfo Open(string filename)
        {
            this.mEBK3Stream = new FileStream(filename, FileMode.Open);
            this.mEBK3Buffer = new byte[this.mEBK3Stream.Length];
            this.mEBK3Stream.Read(this.mEBK3Buffer, 0, this.mEBK3Buffer.Length);
            return Open(this.mEBK3Buffer);
        }

        public BookInfo Open(byte[] buffer)
        {
            EBK3HeadStruct headStruct = new EBK3HeadStruct();
            EBK3HeadInfoModel recodeModel = new EBK3HeadInfoModel();
            ChapterHeadModelStruct chapterHeadModelStruct = new ChapterHeadModelStruct();
            new blockinfoModel();
            new ChapterListModel();
            DynDataStruct dynDataStruct = new DynDataStruct();
            headStruct = new ByteToStructEbk3().GetHeadStruct(buffer);
            int key = (int)headStruct.decode_key;
            byte[] buffer2 = new byte[headStruct.header_length];
            Array.Copy(buffer, 0, buffer2, 0, buffer2.Length);
            byte[] data = new byte[buffer2.Length - 0x10];
            Array.Copy(buffer, 0x10, data, 0, data.Length);
            recodeModel = GetRecodeModel(this.ebk3_data.pbk_sys_data_decode(data, key), (int)recodeModel.modellenth);
            byte[] buffs = new byte[0x10];
            Array.Copy(buffer, (int)recodeModel.chapter_dyn_info_offset5, buffs, 0, buffs.Length);
            dynDataStruct = new ByteToStructEbk3().GetDynDataStruct(buffs);
            byte[] buffer5 = new byte[dynDataStruct.chapter_list_length];
            Array.Copy(buffer, (int)dynDataStruct.chapter_list_offset, buffer5, 0, buffer5.Length);
            chapterHeadModelStruct = new ByteToStructEbk3().GetChapterHeadModelStruct(buffer5);
            byte[] buff = new byte[buffer5.Length - 8];
            Array.Copy(buffer5, 8, buff, 0, buff.Length);
            this.mChapterList = this.GetChapterList((int)chapterHeadModelStruct.chapter_count, (int)chapterHeadModelStruct.chapter_name_header_length, buff, key);
            this.mChapterList = this.SortChapterList(this.mChapterList);
            BookInfo info = new BookInfo();
            for (int i = 0; i < this.mChapterList.Count; i++)
            {
                ChapterListModel model2 = this.mChapterList[i];
                if ((model2.chapter_data_type3 != 0) && (info.Text == ""))
                {
                    byte[] blockdata = new byte[buffer.Length - model2.chapter_data_block_offset4];
                    Array.Copy(buffer, (int)model2.chapter_data_block_offset4, blockdata, 0, blockdata.Length);
                    string chapter = this.Getchapter(blockdata, 1);
                    info.Text = chapter;
                }
            }
            byte[] bytes = Encoding.Unicode.GetBytes(info.Text);
            int num3 = 0;
            int myindex = 0;
            for (int j = 0; j < this.mChapterList.Count; j++)
            {
                ChapterListModel model3 = this.mChapterList[j];
                if (model3.chapter_data_type3 == 1)
                {
                    if (num3 == 0)
                    {
                        BookItems items = new BookItems(model3.chapter_name_data8, 0, model3.chapter_name_data8.Length);
                        info.ChaterList.Add(items);
                    }
                    else
                    {
                        ChapterListModel model4 = this.mChapterList[j - 1];
                        int num6 = (int)model4.chapter_content_decompresss_offset5;
                        int num7 = (int)model4.chapter_content_decompress_length6;
                        byte[] buffer9 = new byte[num7];
                        Array.Copy(bytes, num6, buffer9, 0, num7);
                        string str2 = Encoding.Unicode.GetString(buffer9, 0, buffer9.Length);
                        myindex += str2.Replace("\r\n", "\n").Length;
                        BookItems items2 = new BookItems(model3.chapter_name_data8, myindex, model3.chapter_name_data8.Length);
                        info.ChaterList.Add(items2);
                    }
                    num3++;
                }
            }
            return info;
        }

        public List<ChapterListModel> SortChapterList(List<ChapterListModel> chapterList)
        {
            ChapterListModel model = null;
            for (int i = chapterList.Count; i > 0; i--)
            {
                for (int j = 0; j < (i - 1); j++)
                {
                    if (chapterList[j].chapter_index1 > chapterList[j + 1].chapter_index1)
                    {
                        model = chapterList[j];
                        chapterList[j] = chapterList[j + 1];
                        chapterList[j + 1] = model;
                    }
                }
                if ((i > 2) && (chapterList[i - 1].chapter_index1 == chapterList[i - 2].chapter_index1))
                {
                    chapterList.RemoveAt(i - 1);
                }
            }
            return chapterList;
        }

        private void writefile(byte[] filebyte)
        {
        }
    }
}