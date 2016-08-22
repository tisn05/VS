namespace Zhangyue.iReader.EBK3
{
    using System;
    using System.Text;

    public class GetEbk3HeadArray
    {
        public object[] infomodelarray(EBK3HeadInfoModel infomodel)
        {
            object[] objArray = new object[infomodel.modellenth];
            EBK3HeadByteModel model = new EBK3HeadByteModel {
                bytemin_version1 = BitConverter.GetBytes(infomodel.min_version1)
            };
            objArray[0] = model.bytemin_version1;
            model.byteunique_identifier2 = Encoding.Unicode.GetBytes(infomodel.unique_identifier2);
            objArray[1] = model.byteunique_identifier2;
            model.bytebook_file_type3 = BitConverter.GetBytes(infomodel.book_file_type3);
            objArray[2] = model.bytebook_file_type3;
            model.bytechapter_list_offset4 = BitConverter.GetBytes(infomodel.chapter_list_offset4);
            objArray[3] = model.bytechapter_list_offset4;
            model.bytechapter_dyn_info_offset5 = BitConverter.GetBytes(infomodel.chapter_dyn_info_offset5);
            objArray[4] = model.bytechapter_dyn_info_offset5;
            model.bytecover_data_offset6 = BitConverter.GetBytes(infomodel.cover_data_offset6);
            objArray[5] = model.bytecover_data_offset6;
            model.bytecreator7 = Encoding.Unicode.GetBytes(infomodel.creator7);
            objArray[6] = model.bytecreator7;
            model.bytecreate_timer8 = Encoding.Unicode.GetBytes(infomodel.create_timer8);
            objArray[7] = model.bytecreate_timer8;
            model.bytename9 = Encoding.Unicode.GetBytes(infomodel.name);
            objArray[8] = model.bytename9;
            model.byteauthor10 = Encoding.Unicode.GetBytes(infomodel.author);
            objArray[9] = model.byteauthor10;
            model.bytesubject11 = Encoding.Unicode.GetBytes(infomodel.subject);
            objArray[10] = model.bytesubject11;
            model.bytepublisher12 = Encoding.Unicode.GetBytes(infomodel.publisher);
            objArray[11] = model.bytepublisher12;
            model.byteisbn13 = Encoding.Unicode.GetBytes(infomodel.isbn13);
            objArray[12] = model.byteisbn13;
            model.bytefile_size14 = BitConverter.GetBytes(infomodel.file_size14);
            objArray[13] = model.bytefile_size14;
            model.byterevise_version16 = BitConverter.GetBytes(infomodel.revise_version16);
            objArray[14] = model.byterevise_version16;
            return objArray;
        }
    }
}

