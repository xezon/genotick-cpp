
#include <igenotick.h>
#include <igenotick_utils.h>

#pragma comment(lib, "genotickcpp.lib")

#define GENOTICK_OUTDIR "D:\\Trading\\Code\\genotick\\.jar\\OUTPUT"
#define GENOTICK_DATADIR "D:\\Trading\\Code\\genotick\\.jar\\spx_data"
#define JAVA_CLASS_PATH "D:\\Trading\\Code\\genotick\\.jar\\genotick.jar"
#define JVM_PATH "C:\\Program Files (x86)\\Java\\jre1.8.0_144\\bin\\client\\jvm.dll"

void GenerateDataPoints(TGenotickDataPoint* dataPoints, TGenotickSize size)
{
	for (TGenotickSize i = 0; i < size; ++i)
	{
		dataPoints[i].time   = 20130101 + i;
		dataPoints[i].open   = size*3 + i * 1.0;
		dataPoints[i].high   = size*3 + i * 1.0 + 0.5;
		dataPoints[i].low    = size*3 + i * 1.0 - 0.5;
		dataPoints[i].close  = size*3 + i * 1.0 + 0.2;
		dataPoints[i].volume = 10000.0;
	}
}

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	IGenotick* pInstance = 0;
	TGenotickLoadSettings jvmSettings = { 0 };
	jvmSettings.utf8_jvmDllPath = JVM_PATH;
	jvmSettings.utf8_javaClassPath = JAVA_CLASS_PATH;

	EGenotickResult result = LoadGenotick(&pInstance, &jvmSettings);

	if (result == GenotickResult_Success)
	{
		TGenotickInt32 version = pInstance->GetInterfaceVersion(pInstance);

		TGenotickInt32 sessionId = 0;
		result = pInstance->CreateSession(pInstance, sessionId);

		TGenotickMainSettings mainSettings = { 0 };
		char buffer1[260] = { 0 };
		char buffer2[260] = { 0 };
		GenotickSetString(&mainSettings.populationDAO, buffer1, sizeof(buffer1));
		GenotickSetString(&mainSettings.dataDirectory, buffer2, sizeof(buffer2));

		result = pInstance->GetSettings(pInstance, sessionId, &mainSettings);

		mainSettings.startTimePoint = 20130101;
		mainSettings.endTimePoint = 20150101;
		GenotickSetConstString(&mainSettings.dataDirectory, GENOTICK_DATADIR);

		result = pInstance->ChangeSettings(pInstance, sessionId, &mainSettings);

		TGenotickDataPoint dataPoints[200];
		TGenotickAssetData assetData;
		assetData.assetName = "ASSET";
		assetData.dataPoints = dataPoints;
		assetData.dataPointCount = GENOTICK_ARRAY_SIZE(dataPoints);
		assetData.firstDataPointIsNewest = GenotickFalse;
		GenerateDataPoints(dataPoints, assetData.dataPointCount);

		result = pInstance->SetAssetData(pInstance, sessionId, &assetData);

		const char* arguments[] =
		{
			"input=external",
			"outdir=" GENOTICK_OUTDIR,
		};
		TGenotickStartArgs startArgs = { 0 };
		startArgs.elements = arguments;
		startArgs.elementCount = GENOTICK_ARRAY_SIZE(arguments);

		result = pInstance->Start(pInstance, sessionId, &startArgs);

		IGenotickTimePoints* pTimePoints = 0;
		IGenotickPredictions* pPredictions = 0;
		TGenotickTimePoint timePoint = 0;
		EGenotickPrediction prediction = GenotickPrediction_Out;
		result = pInstance->GetTimePoints(pInstance, sessionId, &pTimePoints);
		result = pInstance->GetPredictions(pInstance, sessionId, "ASSET", &pPredictions);
		result = pInstance->GetNewestTimePoint(pInstance, sessionId, &timePoint);
		result = pInstance->GetNewestPrediction(pInstance, sessionId, "ASSET", &prediction);

		result = pInstance->RemoveSession(pInstance, sessionId);

		GENOTICK_SAFE_RELEASE(pInstance);
	}

	return 0;
}
