Tôi là **Đường Tâm** đây.

Đây là Source port từ jx1 cocos v2 bản chuẩn lên Axmol (fork từ cocos v4).
Source run chủ yên trên Android Studio.

Yêu cầu:
  + Cài Axmol 2.1.5 từ [Axmol page](https://github.com/axmolengine/axmol).
  + IDE: Android Studio.
  + Github.

Build Source:
  + Clone source: `git clone https://github.com/locthanhtx/ax-jx1client.git`.
  + Sửa file `path_install_axmol\axmol\core\ui\UIButton.h`
      - Thêm
        ```
        bool getCustomState() const { return _customState; }
        void setCustomState(bool state) { _customState = state; }
        void setItemDwidx(unsigned int ver) { _itemDwidx = ver; };
        unsigned int getItemDwidx() {return _itemDwidx;};

        private:
          bool _customState;
          unsigned int _itemDwidx;
        ```
  + Open Android Studio để cài SDK, NDK, CMake.
  + Open Project để cài tự động mấy cần thiết.
  + Build game thôi.

Link tham khảo: 
  + https://github.com/axmolengine/axmol
