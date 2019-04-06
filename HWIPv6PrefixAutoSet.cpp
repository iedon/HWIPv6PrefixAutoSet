// HWIPv6PrefixAutoSet.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "CCurlTask.h"
#include <Windows.h>

std::string last_ipv6_addr("-");
std::string & replace_all(std::string & str, const std::string & old_value, const std::string & new_value);

int main()
{
	while (true) {

		CCurlTask crawl;
		std::string cookie;

		printf("[登录] 正在获取随机 Token...\r\n");
		if (crawl.Exec(false, "/asp/GetRandCount.asp", "", true, "")) {

			std::string token(crawl.GetResult() + sizeof("\r\n"));
			printf("[登录] 获取 Token 为: [%s], 大小: %zu\r\n", token.c_str(), token.length());

			if (!token.empty()) {

				printf("[登录] 正在登录路由器...\r\n");
				if (crawl.Exec(true, "/login.cgi", "", true, std::string("UserName=").append(USER_NAME).append("&PassWord=").append(USER_ENCRYPTED_PASSWORD).append("&x.X_HW_Token=").append(token))) {

					cookie = crawl.GetResultString();
					printf("[登录] 登录成功\r\n");

					if (!cookie.empty()) {

						size_t cookie_begin_pos = cookie.find("Cookie=");
						if (cookie_begin_pos != std::string::npos) {

							size_t cookie_end_pos = cookie.find("\r\n", cookie_begin_pos);
							if (cookie_end_pos != std::string::npos) {

								cookie = cookie.substr(cookie_begin_pos, cookie_end_pos - cookie_begin_pos);
								if (!cookie.empty()) {

									printf("[登录] 获取 Cookie 为: [%s]\r\n", cookie.c_str());
									printf("[验证] 正在读取当前 IPv6 信息...\r\n");

									if (crawl.Exec(false, "/html/bbsp/common/wanipv6state.asp", cookie)) {

										std::string html = crawl.GetResultString();

										size_t native_ipv6_addr_begin_pos = html.find("\"Preferred\",\"AutoConfigured\",\"");

										if (native_ipv6_addr_begin_pos != std::string::npos) {

											size_t native_ipv6_addr_end_pos = html.find("\"", native_ipv6_addr_begin_pos + sizeof("\"Preferred\",\"AutoConfigured\",\""));
											if (native_ipv6_addr_end_pos != std::string::npos) {

												std::string ipv6_addr = html.substr(native_ipv6_addr_begin_pos + sizeof("\"Preferred\",\"AutoConfigured\",\"") - 1, native_ipv6_addr_end_pos - native_ipv6_addr_begin_pos - sizeof("\"Preferred\",\"AutoConfigured\",\"") + 1);
												if (!ipv6_addr.empty()) {

													std::string show_ipv6(ipv6_addr);
													replace_all(show_ipv6, "\\x3a", ":");
													std::string show_last_ipv6(last_ipv6_addr);
													replace_all(show_last_ipv6, "\\x3a", ":");
													printf("[验证] IPv6 当前地址为: [%s], 上次地址为: [%s]\r\n", show_ipv6.c_str(), show_last_ipv6.c_str());

													if (ipv6_addr != last_ipv6_addr) {

														size_t split = ipv6_addr.find("\\");
														split = ipv6_addr.find("\\", split + 1);
														split = ipv6_addr.find("\\", split + 1);
														split = ipv6_addr.find("\\", split + 1);
														std::string prefix = ipv6_addr.substr(0, split);
														replace_all(prefix, "\\x3a", "%3A");
														prefix.append("%3A%3A%2F64");

														std::string show_prefix(prefix);
														replace_all(show_prefix, "%3A", ":");
														replace_all(show_prefix, "%2F", "/");
														printf("[验证] 请求更新前缀中 [%s]...\r\n", show_prefix.c_str());

														printf("[更新] 正在获取操作 Token...\r\n");
														if (crawl.Exec(false, "/html/bbsp/lanaddress/lanaddress.asp", cookie)) {

															std::string html = crawl.GetResultString();

															size_t operation_token_begin_pos = html.find("id=\"hwonttoken\"\x20value=\"");
															if (operation_token_begin_pos != std::string::npos) {

																size_t operation_token_end_pos = html.find("\">", operation_token_begin_pos);
																if (operation_token_end_pos != std::string::npos) {

																	std::string operation_token = html.substr(operation_token_begin_pos + sizeof("id=\"hwonttoken\"\x20value=\"") - 1, operation_token_end_pos - operation_token_begin_pos - sizeof("id=\"hwonttoken\"\x20value=\"") + 1);
																	printf("[更新] 获取 Token 为: [%s], 大小: %zu\r\n", operation_token.c_str(), operation_token.length());

																	if (!operation_token.empty()) {

																		std::string post_data(std::string("x.IPv6Address=fe80%3A%3A1&y.Prefix=").append(prefix).append("&y.PreferredLifeTime=3600&y.ValidLifeTime=7200&y.Mode=Static&y.ParentPrefix=&y.ChildPrefixMask=%3A%3A%2F64&m.MTU=1472&z.LanInterface=&m.mode=Auto&m.Enable=1&r.Enable=1&p.IPv6DNSConfigType=HGWProxy&p.IPv6DNSWANConnection=&p.IPv6DNSServers=fe80%3A%3A1&n.ULAmode=Disable&x.X_HW_Token=").append(operation_token));
																		if (crawl.Exec(false, "/html/bbsp/lanaddress/set.cgi?x=InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_HW_IPv6Interface.1.IPv6Address.1&y=InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_HW_IPv6Interface.1.IPv6Prefix.1&z=InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_HW_IPv6Interface.1&p=InternetGatewayDevice.LANDevice.1.X_HW_IPv6Config&m=InternetGatewayDevice.LANDevice.1.X_HW_RouterAdvertisement&r=InternetGatewayDevice.LANDevice.1.X_HW_DHCPv6.Server&n=InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_HW_IPv6Interface.1.ULAIPv6Prefix&RequestFile=html/bbsp/lanaddress/lanaddress.asp", cookie, true, post_data)) {

																			if (crawl.GetResultString().find("getErrStrId") == std::string::npos) {

																				printf("[更新] 已经成功更新 IPv6 前缀, 休眠 1 min...\r\n");
																				last_ipv6_addr = ipv6_addr;
																			} // Ahaha
																		} // Ahaha
																	} // Ahaha
																} // Ahaha
															} // Ahaha
														} // Ahaha
													} // Ahaha
												} // Ahaha
											} // Ahaha
										} // Ahaha
									} // Ahaha
								} // Ahaha
							} // Ahaha
						} // Ahaha
					} // Ahaha
				} // Ahaha
			} // Ahaha
		} // Ahaha
		crawl.Exec(false, "/logout.cgi?RequestFile=html/logout.html", cookie);
		printf("==============================================\r\n");
		::Sleep(60000);
	}

    return 0;
}

std::string & replace_all(std::string & str, const std::string & old_value, const std::string & new_value)
{
	while (true)
	{
		std::string::size_type pos(0);
		if ((pos = str.find(old_value)) != std::string::npos)
		{
			str.replace(pos, old_value.length(), new_value);
		}
		else { break; }
	}
	return str;
}
