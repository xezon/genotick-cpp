
#pragma once

#include <genotick/interface.h>
#include <genotick/loader.h>
#include <genotick/jni/remote/main_interface.h>
#include <genotick/jni/remote/main_settings.h>
#include <genotick/jni/remote/data_lines.h>
#include <genotick/jni/remote/main_app_data.h>
#include <genotick/jni/remote/timepoint.h>
#include <genotick/jni/remote/timepoints.h>
#include <genotick/jni/remote/weight_mode.h>
#include <genotick/jni/remote/inherited_weight_mode.h>
#include <genotick/jni/remote/chart_mode.h>
#include <genotick/jni/remote/error_code.h>
#include <genotick/jni/remote/prediction.h>
#include <genotick/jni/remote/predictions.h>
#include <genotick/jni/exceptions.h>
#include <strlcpy.h>

namespace genotick {
namespace jni {

class CGenotick : public IGenotickDestructable
{
public:
	CGenotick(CLoader* pLoader, ::jni::JavaVM* pJavaVM, ::jni::JNIEnv* pJavaEnv);
	virtual ~CGenotick();

private:
	static TGenotickInt32 GENOTICK_CALL GetInterfaceVersion(IGenotick* pThis) {
		return static_cast<const CGenotick*>(pThis)->GetInterfaceVersionInternal();
	}
	static EGenotickResult GENOTICK_CALL CreateSession(IGenotick* pThis, TGenotickSessionId sessionId) {
		return static_cast<const CGenotick*>(pThis)->CreateSessionInternal(sessionId);
	}
	static EGenotickResult GENOTICK_CALL RemoveSession(IGenotick* pThis, TGenotickSessionId sessionId) {
		return static_cast<const CGenotick*>(pThis)->RemoveSessionInternal(sessionId);
	}
	static EGenotickResult GENOTICK_CALL RemoveAllSessions(IGenotick* pThis) {
		return static_cast<const CGenotick*>(pThis)->RemoveAllSessionsInternal();
	}
	static EGenotickResult GENOTICK_CALL GetSettings(IGenotick* pThis, TGenotickSessionId sessionId, TGenotickMainSettings* pSettings) {
		return static_cast<const CGenotick*>(pThis)->GetSettingsInternal(sessionId, pSettings);
	}
	static EGenotickResult GENOTICK_CALL ChangeSettings(IGenotick* pThis, TGenotickSessionId sessionId, const TGenotickMainSettings* pSettings) {
		return static_cast<const CGenotick*>(pThis)->ChangeSettingsInternal(sessionId, pSettings);
	}
	static EGenotickResult GENOTICK_CALL SetAssetData(IGenotick* pThis, TGenotickSessionId sessionId, const TGenotickAssetData* pAssetData) {
		return static_cast<const CGenotick*>(pThis)->SetAssetDataInternal(sessionId, pAssetData);
	}
	static EGenotickResult GENOTICK_CALL Start(IGenotick* pThis, TGenotickSessionId sessionId, const TGenotickStartArgs* pArgs) {
		return static_cast<const CGenotick*>(pThis)->StartInternal(sessionId, pArgs);
	}
	static EGenotickResult GENOTICK_CALL GetTimePoints(IGenotick* pThis, TGenotickSessionId sessionId, IGenotickTimePoints** ppTimePoints) {
		return static_cast<const CGenotick*>(pThis)->GetTimePointsInternal(sessionId, ppTimePoints);
	}
	static EGenotickResult GENOTICK_CALL GetPredictions(IGenotick* pThis, TGenotickSessionId sessionId, IGenotickPredictions** ppPredictions) {
		return static_cast<const CGenotick*>(pThis)->GetPredictionsInternal(sessionId, ppPredictions);
	}
	static EGenotickResult GENOTICK_CALL GetNewestTimePoint(IGenotick* pThis, TGenotickSessionId sessionId, TGenotickTimePoint* pTimePoint) {
		return static_cast<const CGenotick*>(pThis)->GetNewestTimePointInternal(sessionId, pTimePoint);
	}
	static EGenotickResult GENOTICK_CALL GetNewestPrediction(IGenotick* pThis, TGenotickSessionId sessionId, EGenotickPrediction* pPrediction) {
		return static_cast<const CGenotick*>(pThis)->GetNewestPredictionInternal(sessionId, pPrediction);
	}
	static EGenotickResult GENOTICK_CALL Release(IGenotick* pThis) {
		return static_cast<const CGenotick*>(pThis)->ReleaseInternal();
	}

	TGenotickInt32 GetInterfaceVersionInternal() const;
	EGenotickResult CreateSessionInternal(TGenotickSessionId sessionId) const;
	EGenotickResult RemoveSessionInternal(TGenotickSessionId sessionId) const;
	EGenotickResult RemoveAllSessionsInternal() const;
	EGenotickResult GetSettingsInternal(TGenotickSessionId sessionId, TGenotickMainSettings* pSettings) const;
	EGenotickResult ChangeSettingsInternal(TGenotickSessionId sessionId, const TGenotickMainSettings* pSettings) const;
	EGenotickResult SetAssetDataInternal(TGenotickSessionId sessionId, const TGenotickAssetData* pAssetData) const;
	EGenotickResult StartInternal(TGenotickSessionId sessionId, const TGenotickStartArgs* pArgs) const;
	EGenotickResult GetTimePointsInternal(TGenotickSessionId sessionId, IGenotickTimePoints** ppTimePoints) const;
	EGenotickResult GetPredictionsInternal(TGenotickSessionId sessionId, IGenotickPredictions** ppPredictions) const;
	EGenotickResult GetNewestTimePointInternal(TGenotickSessionId sessionId, TGenotickTimePoint* pTimePoint) const;
	EGenotickResult GetNewestPredictionInternal(TGenotickSessionId sessionId, EGenotickPrediction* pPrediction) const;
	EGenotickResult ReleaseInternal() const;

	template <class D, class S> void ToNative(D& dst, const S src) const {
		dst = static_cast<D>(src);
	}

	template <> void ToNative(TGenotickString& dst, const ::jni::String src) const {
		if (dst.capacity > 0u) {
			std::string buf = ::jni::Make<std::string>(m_javaEnv, src);
			strlcpy(dst.utf8_buffer, buf.c_str(), dst.capacity);
		}
	}

	template <> void ToNative(TGenotickTimePoint& dst, const remote::CTimePoint::TObject src) const {
		dst = static_cast<TGenotickTimePoint>(m_timePoint.getValue(src));
	}

	template <> void ToNative(EGenotickWeightMode& dst, const remote::CWeightMode::TObject src) const {
		dst = EGenotickWeightMode::getByValue(m_weightMode.GetEnumValue(src));
	}

	template <> void ToNative(EGenotickInheritedWeightMode& dst, const remote::CInheritedWeightMode::TObject src) const {
		dst = EGenotickInheritedWeightMode::getByValue(m_inheritedWeightMode.GetEnumValue(src));
	}

	template <> void ToNative(EGenotickChartMode& dst, const remote::CChartMode::TObject src) const {
		dst = EGenotickChartMode::getByValue(m_chartMode.GetEnumValue(src));
	}

	template <class D, class S> D ToJava(const S src) const {
		return static_cast<D>(src);
	}

	template <> ::jni::String ToJava(const TGenotickString src) const {
		const std::string buf = (src.utf8_buffer != nullptr) ? src.utf8_buffer : "";
		return ::jni::Make<::jni::String>(m_javaEnv, buf);
	}

	template <> remote::CTimePoint::TObject ToJava(const TGenotickTimePoint src) const {
		return m_timePoint.New(static_cast<::jni::jlong>(src));
	}

	template <> remote::CWeightMode::TObject ToJava(const EGenotickWeightMode src) const {
		return m_weightMode.GetEnumObject(src.value());
	}

	template <> remote::CInheritedWeightMode::TObject ToJava(const EGenotickInheritedWeightMode src) const {
		return m_inheritedWeightMode.GetEnumObject(src.value());
	}

	template <> remote::CChartMode::TObject ToJava(const EGenotickChartMode src) const {
		return m_chartMode.GetEnumObject(src.value());
	}

	CLoaderFriend& m_loader;
	JavaVM& m_javaVM;
	JNIEnv& m_javaEnv;
	
	::jni::UniqueStringClass m_stringClass;
	remote::CMainInterface m_mainInterface;
	remote::CMainSettings m_mainSettings;
	remote::CDataLines m_dataLines;
	remote::CMainAppData m_mainAppData;
	remote::CTimePoint m_timePoint;
	remote::CTimePoints m_timePoints;
	remote::CWeightMode m_weightMode;
	remote::CInheritedWeightMode m_inheritedWeightMode;
	remote::CChartMode m_chartMode;
	remote::CErrorCode m_errorCode;
	remote::CPrediction m_prediction;
	remote::CPredictions m_predictions;
};

} // namespace jni
} // namespace genotick
