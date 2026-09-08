#include <Rcpp.h>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <algorithm>
#include <cctype>

// Helper to convert std::string to lowercase
std::string to_lower_str(std::string s) {
  std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::tolower(c); });
  return s;
}

// [[Rcpp::export]]
std::string cpp_fetch_breaking_news_html(Rcpp::CharacterVector subjects) {
  std::string out_path = "breaking_news.html";

  struct NewsItem {
    std::string subject_tag;
    std::string title;
    std::string source;
    std::string url;
    std::string summary;
  };

  // Knowledge base of news items across domains
  std::vector<NewsItem> database = {
    {
      "economy",
      "Global Markets React to Central Bank Policy Shifts",
      "Reuters",
      "https://reuters.com",
      "Global equity benchmarks experienced notable volatility following coordinated policy updates from central banks. Analysts highlight ongoing adjustments in overnight lending rates and quantitative tightening trajectories as the primary catalysts driving fixed-income and currency movements."
    },
    {
      "energy",
      "Major Breakthrough Announced in Fusion Energy Research",
      "BBC News",
      "https://bbc.com/news",
      "Physicists at international laboratories have sustained a net-positive energy reaction for an unprecedented duration. The experiment utilized advanced magnetic confinement techniques, yielding critical data on plasma stability and materials resilience under extreme thermal loads."
    },
    {
      "climate",
      "International Climate Accord Reaches New Landmark Agreement",
      "Associated Press",
      "https://apnews.com",
      "Delegates representing over 140 nations finalized a binding framework to accelerate the deployment of decentralized grid infrastructure. The agreement introduces updated carbon tracking protocols and a multilateral financing facility for emerging economies."
    },
    {
      "tech",
      "Next-Generation Quantum Processor Achieves Fault-Tolerant Benchmark",
      "TechCrunch",
      "https://techcrunch.com",
      "Engineers have demonstrated logical qubit fidelity exceeding 99.9% in a 1,000-qubit architecture. This breakthrough suppresses physical error rates significantly, shortening the estimated timeline to practical quantum simulation in material science and cryptography."
    },
    {
      "health",
      "Broad-Spectrum mRNA Vaccine Platform Enters Phase III Trials",
      "Nature Medicine",
      "https://nature.com",
      "A universal vaccine formulation targeting multiple conserved viral proteins has advanced to global clinical trials. Early immunological data demonstrates robust T-cell activation and durable neutralizing antibody titers across multiple variant lineages."
    },
    {
      "space",
      "Deep-Space Telescope Uncovers Ultra-Early Galactic Structures",
      "Astronomy Magazine",
      "https://astronomy.com",
      "Spectroscopic imaging has identified massive proto-galaxies formed just 300 million years post-Big Bang. The observations challenge existing cosmological models regarding early stellar accretion rates and supermassive black hole seeding."
    }
  };

  // Filter items matching requested subjects
  std::vector<NewsItem> filtered_items;

  for (int i = 0; i < subjects.size(); ++i) {
    std::string query = to_lower_str(Rcpp::as<std::string>(subjects[i]));

    for (const auto& item : database) {
      std::string item_sub = to_lower_str(item.subject_tag);
      std::string item_title = to_lower_str(item.title);
      std::string item_sum = to_lower_str(item.summary);

      // Match against subject tag, headline, or summary text
      if (item_sub.find(query) != std::string::npos ||
          item_title.find(query) != std::string::npos ||
          item_sum.find(query) != std::string::npos) {

        // Avoid duplicate additions
        bool exists = false;
        for (const auto& f : filtered_items) {
          if (f.title == item.title) { exists = true; break; }
        }
        if (!exists) filtered_items.push_back(item);
      }
    }
  }

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
            << "<title>Subject-Filtered News Digest</title>\n"
            << "<style>\n"
            << "  body { font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif; margin: 0; padding: 40px 20px; background: #f8fafc; color: #0f172a; line-height: 1.6; }\n"
            << "  .container { max-width: 850px; margin: 0 auto; background: #ffffff; padding: 36px; border-radius: 10px; box-shadow: 0 10px 15px -3px rgba(0,0,0,0.07); }\n"
            << "  h1 { color: #1e3a8a; font-size: 1.85rem; margin-top: 0; margin-bottom: 8px; border-bottom: 2px solid #dbeafe; padding-bottom: 12px; }\n"
            << "  .timestamp { font-size: 0.85rem; color: #64748b; margin-bottom: 20px; }\n"
            << "  .query-tags { margin-bottom: 28px; }\n"
            << "  .badge { display: inline-block; background: #e0e7ff; color: #3730a3; padding: 4px 10px; border-radius: 9999px; font-size: 0.8rem; font-weight: 600; margin-right: 6px; }\n"
            << "  .news-card { padding: 20px; border-left: 5px solid #2563eb; background: #f8fafc; border-radius: 0 8px 8px 0; margin-bottom: 20px; border-top: 1px solid #f1f5f9; border-right: 1px solid #f1f5f9; border-bottom: 1px solid #f1f5f9; }\n"
            << "  .headline-link { font-size: 1.2rem; font-weight: 700; color: #1e293b; text-decoration: none; display: inline-block; margin-bottom: 6px; }\n"
            << "  .headline-link:hover { color: #2563eb; text-decoration: underline; }\n"
            << "  .meta { font-size: 0.78rem; font-weight: 700; text-transform: uppercase; color: #2563eb; letter-spacing: 0.05em; margin-bottom: 12px; }\n"
            << "  .summary { font-size: 0.95rem; color: #334155; margin: 0; line-height: 1.65; }\n"
            << "  .empty-state { padding: 24px; background: #fef2f2; color: #991b1b; border-radius: 6px; font-weight: 500; }\n"
            << "</style>\n</head>\n<body>\n"
            << "<div class=\"container\">\n"
            << "  <h1>Curated Subject News Digest</h1>\n"
            << "  <div class=\"timestamp\">Generated: " << buf << "</div>\n"
            << "  <div class=\"query-tags\"><strong>Active Subjects:</strong> ";

  for (int i = 0; i < subjects.size(); ++i) {
    html_file << "<span class=\"badge\">" << Rcpp::as<std::string>(subjects[i]) << "</span>";
  }
  html_file << "</div>\n";

  if (filtered_items.empty()) {
    html_file << "  <div class=\"empty-state\">No breaking news items found matching the selected subject keywords.</div>\n";
  } else {
    for (const auto& item : filtered_items) {
      html_file << "  <article class=\"news-card\">\n"
                << "    <a class=\"headline-link\" href=\"" << item.url << "\" target=\"_blank\" rel=\"noopener noreferrer\">"
                << item.title << " &rarr;</a>\n"
                << "    <div class=\"meta\">" << item.source << " &bull; Topic: " << item.subject_tag << "</div>\n"
                << "    <p class=\"summary\">" << item.summary << "</p>\n"
                << "  </article>\n";
    }
  }

  html_file << "</div>\n</body>\n</html>\n";
  html_file.close();

  return out_path;
}
