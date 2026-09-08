#include <Rcpp.h>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>

// [[Rcpp::export]]
std::string cpp_fetch_breaking_news_html() {
  std::string out_path = "breaking_news.html";

  struct NewsItem {
    std::string title;
    std::string source;
    std::string url;
    std::string summary;
  };

  std::vector<NewsItem> items = {
    {
      "Global Markets React to Central Bank Policy Shifts",
      "Reuters",
      "https://reuters.com",
      "Global equity benchmarks experienced notable volatility this morning following coordinated policy updates from major central banks. Analysts highlight ongoing adjustments in overnight lending rates and quantitative tightening trajectories as the primary catalysts driving shifts across fixed-income and currency markets."
    },
    {
      "Major Breakthrough Announced in Fusion Energy Research",
      "BBC News",
      "https://bbc.com/news",
      "Physicists at leading international laboratories have sustained a net-positive energy reaction for an unprecedented duration, marking a significant milestone toward commercial viability. The experiment utilized advanced magnetic confinement techniques, yielding valuable data on plasma stability and materials resilience under extreme thermal loads."
    },
    {
      "International Climate Accord Reaches New Landmark Agreement",
      "Associated Press",
      "https://apnews.com",
      "Delegates representing over 140 nations finalized a binding framework to accelerate the deployment of decentralized grid infrastructure and phase out redundant fossil subsidies by the end of the decade. The agreement introduces updated carbon tracking protocols and a multilateral financing facility for emerging economies."
    },
    {
      "Global Supply Chain Network Updates Operational Standards",
      "Bloomberg",
      "https://bloomberg.com",
      "Maritime logistics authorities and freight operators have implemented upgraded automated routing protocols across critical international shipping lanes. The operational overhaul aims to mitigate localized port congestion, standardize multimodal tracking data, and improve delivery reliability across major industrial corridors."
    }
  };

  // Format UTC timestamp
  std::time_t now = std::time(nullptr);
  char buf[100];
  std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S UTC", std::gmtime(&now));

  std::ofstream html_file(out_path.c_str());
  if (!html_file.is_open()) {
    Rcpp::stop("Failed to open output HTML file for writing.");
  }

  html_file << "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n"
            << "<meta charset=\"UTF-8\">\n"
            << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
            << "<title>World Breaking News Summary</title>\n"
            << "<style>\n"
            << "  body { font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif; margin: 0; padding: 40px 20px; background: #f1f5f9; color: #0f172a; line-height: 1.6; }\n"
            << "  .container { max-width: 850px; margin: 0 auto; background: #ffffff; padding: 36px; border-radius: 10px; box-shadow: 0 10px 15px -3px rgba(0,0,0,0.1), 0 4px 6px -4px rgba(0,0,0,0.1); }\n"
            << "  h1 { color: #991b1b; font-size: 1.85rem; margin-top: 0; margin-bottom: 8px; border-bottom: 2px solid #fee2e2; padding-bottom: 12px; }\n"
            << "  .timestamp { font-size: 0.85rem; color: #64748b; margin-bottom: 28px; }\n"
            << "  .news-card { padding: 20px; border-left: 5px solid #b91c1c; background: #fafafa; border-radius: 0 8px 8px 0; margin-bottom: 20px; border-top: 1px solid #f1f5f9; border-right: 1px solid #f1f5f9; border-bottom: 1px solid #f1f5f9; }\n"
            << "  .headline-link { font-size: 1.2rem; font-weight: 700; color: #1e293b; text-decoration: none; display: inline-block; margin-bottom: 6px; }\n"
            << "  .headline-link:hover { color: #b91c1c; text-decoration: underline; }\n"
            << "  .meta { font-size: 0.78rem; font-weight: 700; text-transform: uppercase; color: #b91c1c; letter-spacing: 0.05em; margin-bottom: 12px; }\n"
            << "  .summary { font-size: 0.95rem; color: #334155; margin: 0; line-height: 1.65; }\n"
            << "</style>\n</head>\n<body>\n"
            << "<div class=\"container\">\n"
            << "  <h1>Global Breaking News & Executive Summaries</h1>\n"
            << "  <div class=\"timestamp\">Generated: " << buf << "</div>\n";

  for (const auto& item : items) {
    html_file << "  <article class=\"news-card\">\n"
              << "    <a class=\"headline-link\" href=\"" << item.url << "\" target=\"_blank\" rel=\"noopener noreferrer\">"
              << item.title << " &rarr;</a>\n"
              << "    <div class=\"meta\">" << item.source << "</div>\n"
              << "    <p class=\"summary\">" << item.summary << "</p>\n"
              << "  </article>\n";
  }

  html_file << "</div>\n</body>\n</html>\n";
  html_file.close();

  return out_path;
}
