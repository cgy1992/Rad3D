
#
# MGui
#

include $(CLEAR_VARS)

LOCAL_MODULE := libMGui
LOCAL_STATIC_LIBRARIES := libFreeType
LOCAL_SHARED_LIBRARIES := libMCore

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../../MCore \
	$(LOCAL_PATH)/../../MCore/Platform/android \
	$(LOCAL_PATH)/../../MGUI \
	$(LOCAL_PATH)/../../ThirdParty/xml \
	$(LOCAL_PATH)/../../ThirdParty/freeType/include
	
	
LOCAL_SRC_FILES := \
	../../MGUI/MGUI_Button.cpp \
	../../MGUI/MGUI_CheckBox.cpp \
	../../MGUI/MGUI_Clipboard.cpp \
	../../MGUI/MGUI_ComboBox.cpp \
	../../MGUI/MGUI_Component.cpp \
	../../MGUI/MGUI_Fade.cpp \
	../../MGUI/MGUI_KFAnimColor.cpp \
	../../MGUI/MGUI_KFAnimImage.cpp \
	../../MGUI/MGUI_KFAnimScale.cpp \
	../../MGUI/MGUI_KFAnimSize.cpp \
	../../MGUI/MGUI_KFAnimMove.cpp \
	../../MGUI/MGUI_KFAnimRect.cpp \
	../../MGUI/MGUI_Timer.cpp \
	../../MGUI/MGUI_EditBox.cpp \
	../../MGUI/MGUI_EditView.cpp \
	../../MGUI/MGUI_Engine.cpp \
	../../MGUI/MGUI_Factory.cpp \
	../../MGUI/MGUI_Font.cpp \
	../../MGUI/MGUI_FontImage.cpp \
	../../MGUI/MGUI_FontManager.cpp \
	../../MGUI/MGUI_FontTTF.cpp \
	../../MGUI/MGUI_GridBox.cpp \
	../../MGUI/MGUI_Helper.cpp \
	../../MGUI/MGUI_Loader.cpp \
	../../MGUI/MGUI_Locale.cpp \
	../../MGUI/MGUI_ScrollBar.cpp \
	../../MGUI/MGUI_SliderBar.cpp \
	../../MGUI/MGUI_InputManager.cpp \
	../../MGUI/MGUI_RenderLayer.cpp \
	../../MGUI/MGUI_ListBoxItem.cpp \
	../../MGUI/MGUI_Node.cpp \
	../../MGUI/MGUI_Panel.cpp \
	../../MGUI/MGUI_ProgressBar.cpp \
	../../MGUI/MGUI_ImageBox.cpp \
	../../MGUI/MGUI_Label.cpp \
	../../MGUI/MGUI_ListBox.cpp \
	../../MGUI/MGUI_RenderItem.cpp \
	../../MGUI/MGUI_ScrollView.cpp \
	../../MGUI/MGUI_LookFeel.cpp \
	../../MGUI/MGUI_TabControl.cpp \
	../../MGUI/MGUI_TabItem.cpp \
	../../MGUI/MGUI_TabPage.cpp \
	../../MGUI/MGUI_TextBox.cpp \
	../../MGUI/MGUI_TextView.cpp \
	../../MGUI/MGUI_TreeViewItem.cpp \
	../../MGUI/MGUI_TreeView.cpp \
	../../MGUI/MGUI_Types.cpp \
	../../MGUI/MGUI_Layout.cpp \
	../../MGUI/MGUI_Widget.cpp \
	../../MGUI/MGUI_WidgetInput.cpp \
	../../MGUI/MGUI_Window.cpp
	
include $(BUILD_SHARED_LIBRARY)
