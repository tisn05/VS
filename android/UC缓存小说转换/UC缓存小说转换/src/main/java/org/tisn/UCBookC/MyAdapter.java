package org.tisn.UCBookC;

import java.util.ArrayList;
import java.util.HashMap;

import android.content.Context;
import android.database.Cursor;
import android.provider.Telephony.Sms.Conversations;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.CheckBox;
import android.widget.TextView;

public class MyAdapter extends BaseAdapter {
	public final class ViewHolder {
		public TextView title;
		public TextView text;
		public CheckBox cBox;
	}

	ArrayList<HashMap<String, Object>> listItem;
	ArrayList<Boolean> Checkeds;
	private LayoutInflater mInflater;

	public MyAdapter(Context context) {
		mInflater = LayoutInflater.from(context);
		listItem = new ArrayList<HashMap<String, Object>>();
		Checkeds = new ArrayList<Boolean>();
		DatabaseContext dc = new DatabaseContext(context);
		BooksDB bd = new BooksDB(dc);
		Cursor cs = bd.getAll();
		while (cs.moveToNext()) {
			HashMap<String, Object> map = new HashMap<String, Object>();
			map.put("ItemTitle", cs.getString(1));
			map.put("ItemText", cs.getString(0));
			listItem.add(map);
			Checkeds.add(false);
		}
	}

	@Override
	public int getCount() {
		// TODO Auto-generated method stub
		return listItem.size();
	}

	@Override
	public HashMap<String, Object> getItem(int position) {
		// TODO Auto-generated method stub
		return listItem.get(position);
	}

	@Override
	public long getItemId(int position) {
		// TODO Auto-generated method stub
		return 0;
	}
	
	public String getBookID(int position){
		return (String) listItem.get(position).get("ItemText");
	}
	
	public Integer[] getSelecteds(){
		ArrayList<Integer> re = new ArrayList<Integer>();
		for(int i=0;i<getCount();i++){
			if(Checkeds.get(i)){
				re.add(i);
			}
		}
		return re.toArray(new Integer[0]);
	}

	@Override
	public View getView(final int position, View convertView, ViewGroup parent) {
		ViewHolder holder = null;
		if (convertView == null) {
			holder = new ViewHolder();
			convertView = mInflater.inflate(R.layout.list_item, null);
			holder.title = (TextView) convertView.findViewById(R.id.ItemTitle);
			holder.text = (TextView) convertView.findViewById(R.id.ItemText);
			holder.cBox = (CheckBox) convertView
					.findViewById(R.id.ItemCheckBox);
			holder.cBox.setTag(position);
			holder.cBox.setOnClickListener(new OnClickListener() {

				@Override
				public void onClick(View v) {
					CheckBox cb = (CheckBox) v;
					Checkeds.set(position, cb.isChecked());
				}
			});
			convertView.setTag(holder);
		} else {
			holder = (ViewHolder) convertView.getTag();
            boolean b = Checkeds.get(position);
            holder.cBox.setChecked(b);
		}
		holder.title.setText((String) getItem(position).get("ItemTitle"));
		holder.text.setText((String) getItem(position).get("ItemText"));
		return convertView;
	}

}
