#ifndef CTP_TRADE_HANDLER
#define CTP_TRADE_HANDLER

#include <thread>

#include "ThostFtdcTraderApi.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "common/common_struct.h"
#include "common/ws_service.h"
#include "common/http_service.h"
#include "conf.h"

using namespace babeltrader;

class CTPTradeHandler : public TradeService, CThostFtdcTraderSpi
{
public:
	CTPTradeHandler(CTPTradeConf &conf);

	void run();

public:
	////////////////////////////////////////
	// trade service virtual function
	virtual void InsertOrder(uWS::WebSocket<uWS::SERVER> *ws, rapidjson::Value &msg) override;
	virtual void CancelOrder(uWS::WebSocket<uWS::SERVER> *ws, rapidjson::Value &msg) override;
	virtual void QueryOrder(uWS::WebSocket<uWS::SERVER> *ws, rapidjson::Value &msg) override;

	////////////////////////////////////////
	// spi virtual function
	virtual void OnFrontConnected() override;
	virtual void OnFrontDisconnected(int nReason) override;
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
	virtual void OnHeartBeatWarning(int nTimeLapse) override;

	virtual void OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
	virtual void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo) override;
	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder) override;
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade) override;

	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	virtual void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

private:
	void RunAPI();
	void RunService();

	////////////////////////////////////////
	// requests
	void DoAuthenticate();
	void DoLogin();
	void DoSettlementConfirm();

	////////////////////////////////////////
	// broadcast or response message
	void BroadcastOrderConfirm(Order &order, int error_id, const char *error_msg);
	void BroadcastOrderStatus(Order &order, OrderStatusNotify &order_status_notify, int error_id, const char *error_msg);
	void BroadcastOrderDeal(Order &order, OrderDealNotify &order_deal);
	void SendRspOrderQry2Client(uWS::WebSocket<uWS::SERVER>* ws, OrderQuery &order_qry, std::vector<CThostFtdcOrderField> &orders, CThostFtdcRspInfoField *pRspInfo);

	////////////////////////////////////////
	// convert json to ctp struct
	void ConvertInsertOrderJson2CTP(rapidjson::Value &msg, CThostFtdcInputOrderField &req);
	void ConvertCancelOrderJson2CTP(rapidjson::Value &msg, CThostFtdcInputOrderActionField &req);
	void ConvertQueryOrderJson2CTP(rapidjson::Value &msg, CThostFtdcQryOrderField &req);

	////////////////////////////////////////
	// convert ctp struct to json
	void SerializeCTPInputOrder(rapidjson::Writer<rapidjson::StringBuffer> &writer, CThostFtdcInputOrderField *pInputOrder);
	void SerializeCTPActionOrder(rapidjson::Writer<rapidjson::StringBuffer> &writer, CThostFtdcInputOrderActionField *pActionOrder);
	void SerializeCTPQueryOrder(rapidjson::Writer<rapidjson::StringBuffer> &writer, CThostFtdcQryOrderField *pQryOrder);
	void SerializeCTPOrder(rapidjson::Writer<rapidjson::StringBuffer> &writer, CThostFtdcOrderField *pOrder);
	void SerializeCTPTrade(rapidjson::Writer<rapidjson::StringBuffer> &writer, CThostFtdcTradeField *pTrade);

	////////////////////////////////////////
	// convert to common
	void ConvertInsertOrderCTP2Common(CThostFtdcInputOrderField &req, Order &order);
	void ConvertInsertOrderJson2Common(rapidjson::Value &msg, Order &order);
	void ConvertRtnOrderCTP2Common(CThostFtdcOrderField *pOrder, Order &order, OrderStatusNotify &order_status_notify);
	void ConvertRtnTradeCTP2Common(CThostFtdcTradeField *pTrade, Order &order, OrderDealNotify &order_deal);
	void ConvertQryOrderJson2Common(rapidjson::Value &msg, OrderQuery &order_qry);

	////////////////////////////////////////
	// output ctp struct 
	void OutputOrderInsert(CThostFtdcInputOrderField *req);
	void OutputOrderAction(CThostFtdcInputOrderActionField *req);
	void OutputOrderQuery(CThostFtdcQryOrderField *req);

	void OutputFrontConnected();
	void OutputFrontDisconnected(int reason);
	void OutputRsperror(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OutputAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OutputRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OutputRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OutputRspSettlementConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OutputRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OutputErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);
	void OutputRtnOrder(CThostFtdcOrderField *pOrder);
	void OutputRtnTrade(CThostFtdcTradeField *pTrade);
	void OutputRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OutputRspOrderQuery(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	char getOrderType(const char *order_type);
	char getOrderFlag1(const char *order_flag1);
	bool getOrderDir(const char *order_dir, char& action, char& dir);

	void FillConnectionInfo(const char *tradeing_day, const char *login_time, int front_id, int session_id);
	void ClearConnectionInfo();

	void RecordOrder(Order &order, const std::string &order_ref, int front_id, int session_id);
	bool GetRecordOrder(Order &order, const std::string &user_id, const std::string &order_ref, int front_id, int session_id);
	bool GetAndCleanRecordOrder(Order &order, const std::string &user_id, const std::string &order_ref, int front_id, int session_id);

	int GetOrderStatus(TThostFtdcOrderStatusType OrderStatus);
	int GetOrderSubmitStatus(TThostFtdcOrderSubmitStatusType OrderSubmitStatus);
	void GetOrderDirection(const char ctp_dir, const char ctp_hedge_flag, const char ctp_offset_flag, Order &order);
	std::string GenOutsideOrderIdFromOrder(CThostFtdcOrderField *pOrder);
	std::string GenOutsideOrderIdFromDeal(CThostFtdcTradeField *pTrade);
	std::string GenOutsideTradeIdFromDeal(CThostFtdcTradeField *pTrade);
	bool GetCTPOrderSysIDFromOutsideId(TThostFtdcOrderSysIDType &ctp_order_sys_id, const char *outside_id, int len);

	std::string GetQryId(rapidjson::Value &msg);

	void CacheQryOrder(int req_id, uWS::WebSocket<uWS::SERVER>* ws, OrderQuery &&order_qry);
	void GetAndClearCacheQryOrder(int req_id, uWS::WebSocket<uWS::SERVER>*& ws, OrderQuery &order_qry);

private:
	CThostFtdcTraderApi *api_;
	bool api_ready_;

	CTPTradeConf conf_;

	uWS::Hub uws_hub_;
	WsService ws_service_;
	HttpService http_service_;

	int req_id_;
	int order_ref_;
	int order_action_ref_;

	// connection info
	std::string ctp_tradeing_day_;
	std::string ctp_login_time_;
	int ctp_front_id_;
	int ctp_session_id_;

	// order recorder
	std::map<std::string, Order> wait_deal_orders_;
	std::mutex wati_deal_order_mtx_;

	// query results
	std::mutex qry_cache_mtx_;
	std::map<int, uWS::WebSocket<uWS::SERVER>*> qry_ws_cache_;
	std::map<int, OrderQuery> qry_info_cache_;

	std::map<int, std::vector<CThostFtdcOrderField>> qry_order_caches_;
};

#endif