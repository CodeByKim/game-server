#include "./Util/Logger/LogManager.h"
#include "./Util/Logger/Log.h"
#include "./Util/Logger/Logger.h"
#include "./Util/Logger/LogWriter.h"
#include "./Util/Logger/ConsoleWriter.h"
#include "./Util/Logger/OutputDebugWriter.h"
#include "./Util/Logger/FileWriter.h"

namespace garam
{	
	namespace logger
	{
		std::unordered_map<std::wstring_view, Logger*> Manager::mLoggers;
		std::unordered_map<eLogWriter, Writer*> Manager::mWriters;

		void Manager::Create(Configure* config)
		{
			/*
			 * Logger ��ü ����
			 */
			Logger* logger = new Logger(config);
			mLoggers.insert(std::pair(config->mLoggerName,
				logger));

			for (auto iter = config->mWriters.begin();
				iter != config->mWriters.end();
				++iter)
			{
				/*
				 * ���� Manager�� �ش��ϴ� Writer�� �ִ��� ã�ƺ���
				 * �̹� ������ Logger�� ����ϰ� �ǳʶ�
				 */
				if (mWriters.find(*iter) != mWriters.end())
				{
					logger->SetWriter(mWriters[*iter]);
					continue;
				}

				/*
				 * �ش��ϴ� Writer�� �����ϱ� ���� �����ؼ� �߰�
				 */
				if (*iter == eLogWriter::Console)
				{
					mWriters.insert(std::pair(*iter, new ConsoleWriter()));
				}
				else if (*iter == eLogWriter::OutputDebug)
				{
					mWriters.insert(std::pair(*iter, new OutputDebugWriter()));
				}
				else if (*iter == eLogWriter::File)
				{
					mWriters.insert(std::pair(*iter, new FileWriter()));
				}

				/*
				 * ���� ������ Writer�� Logger�� ���
				 */
				logger->SetWriter(mWriters[*iter]);
			}
		}

		Logger* Manager::Get(std::wstring_view name)
		{
			return mLoggers[name];
		}

		void Manager::Release()
		{
			for (auto& writer : mWriters)
			{
				writer.second->Release();
				delete writer.second;
			}

			for (auto& logger : mLoggers)
			{
				delete logger.second;
			}
		}
	}	
/* 
 * end of namespace
 */
}