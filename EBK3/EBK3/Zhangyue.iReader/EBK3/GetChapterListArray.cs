namespace Zhangyue.iReader.EBK3
{
    using System;
    using System.Text;

    public class GetChapterListArray
    {
        public byte[] infomodelarray(ChapterListModel infomodel, int key)
        {
            Ebk3_Data data = new Ebk3_Data();
            object[] objArray = new object[infomodel.chaptermodellenth];
            ChapterListByteModel model = new ChapterListByteModel {
                chapter_index1 = BitConverter.GetBytes(infomodel.chapter_index1),
                chapter_level2 = BitConverter.GetBytes(infomodel.chapter_level2),
                chapter_data_type3 = BitConverter.GetBytes(infomodel.chapter_data_type3)
            };
            byte[] bytes = BitConverter.GetBytes(infomodel.chapter_data_block_offset4);
            model.chapter_data_block_offset4 = data.pbk_sys_data_encode(bytes, key);
            model.chapter_content_decompresss_offset5 = BitConverter.GetBytes(infomodel.chapter_content_decompresss_offset5);
            model.chapter_content_decompress_length6 = BitConverter.GetBytes(infomodel.chapter_content_decompress_length6);
            model.chapter_name_length7 = BitConverter.GetBytes(infomodel.chapter_name_length7);
            byte[] buffer2 = Encoding.Unicode.GetBytes(infomodel.chapter_name_data8);
            model.chapter_name_data8 = data.pbk_sys_data_encode(buffer2, key);
            objArray[0] = model.chapter_name_length7;
            objArray[1] = model.chapter_index1;
            objArray[2] = model.chapter_level2;
            objArray[3] = model.chapter_data_type3;
            objArray[4] = model.chapter_data_block_offset4;
            objArray[5] = model.chapter_content_decompresss_offset5;
            objArray[6] = model.chapter_content_decompress_length6;
            objArray[7] = model.chapter_name_data8;
            byte[] buffer3 = new byte[0x19000];
            int num = 0;
            for (int i = 0; i < objArray.Length; i++)
            {
                byte[] buffer4 = objArray[i] as byte[];
                Array.Copy(buffer4, 0, buffer3, num, buffer4.Length);
                num += buffer4.Length;
            }
            byte[] buffer5 = new byte[num];
            Array.Copy(buffer3, 0, buffer5, 0, buffer5.Length);
            return buffer5;
        }
    }
}

