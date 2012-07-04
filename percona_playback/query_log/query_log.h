/* BEGIN LICENSE
 * Copyright (C) 2011 Percona Inc.
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2, as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 * END LICENSE */

#ifndef PERCONA_PLAYBACK_QUERY_LOG_H
#define PERCONA_PLAYBACK_QUERY_LOG_H

#include <percona_playback/visibility.h>
#include "percona_playback/query_entry.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "tbb/atomic.h"

PERCONA_PLAYBACK_API
int run_query_log(const std::string &log_file, unsigned int read_count, struct percona_playback_run_result *r);

#ifdef __cplusplus
extern "C"
{
#endif

class DBThread;

class QueryLogEntry : public QueryEntry
{
private:
  uint64_t rows_sent;
  uint64_t rows_examined;
  double query_time;
  std::vector<std::string> info;
  std::vector<std::string> query;
public:

  QueryLogEntry() : rows_sent(0), rows_examined(0), query_time(0) {}

  double getQueryTime() { return query_time; }

  void add_line(const std::string &s, tbb::atomic<uint64_t> *queries);

  std::vector<std::string> *queries() {return &query; };

  void display()
  {
    std::vector<std::string>::iterator it;

    for ( it=query.begin() ; it < query.end(); it++ )
      std::cerr << "    " << *it << std::endl;
  }

  bool is_quit()
  {
    return query.size() &&
      (query[0].compare(0, 30, "# administrator command: Quit;") == 0
       || (query.size()>1 && query[1].compare(0,30,"# administrator command: Quit;") == 0));
  }

  void execute(DBThread *t);
};


#ifdef __cplusplus
}
#endif

#endif /* PERCONA_PLAYBACK_QUERY_LOG_H */
