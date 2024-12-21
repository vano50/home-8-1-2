#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <sstream>
#include <map>

class SqlSelectQueryBuilder {
private:
    std::vector<std::string> columns;
    std::vector<std::string> where_conditions;
    std::string from_table;
    bool has_columns;
public:
    SqlSelectQueryBuilder()
        : has_columns(false) {}

    SqlSelectQueryBuilder& AddColumn(const std::string& column) {
        columns.push_back(column);
        has_columns = true;
        return *this;
    }

    SqlSelectQueryBuilder& AddFrom(const std::string& table) {
        from_table = table;
        return *this;
    }

    SqlSelectQueryBuilder& AddWhere(const std::string& column, const std::string& value) {
        where_conditions.push_back(column + "=" + value);
        return *this;
    }

    SqlSelectQueryBuilder& AddWhere(const std::map<std::string, std::string>& kv) noexcept {
        for (const auto& pair : kv) {
            where_conditions.push_back(pair.first + "=" + pair.second);
        }
        return *this;
    }

    SqlSelectQueryBuilder& AddColumns(const std::vector<std::string>& new_columns) noexcept {
        for (const auto& column : new_columns) {
            columns.push_back(column);
        }
        has_columns = true;
        return *this;
    }

    std::string BuildQuery() {
        std::ostringstream query;
        query << "SELECT ";

        if (columns.empty()) {
            query << "*";
        }
        else {
            for (size_t i = 0; i < columns.size(); ++i) {
                query << columns[i];
                if (i != columns.size() - 1) {
                    query << ", ";
                }
            }
        }

        query << " FROM " << from_table;

        if (!where_conditions.empty()) {
            query << " WHERE ";
            for (size_t i = 0; i < where_conditions.size(); ++i) {
                query << where_conditions[i];
                if (i != where_conditions.size() - 1) {
                    query << " AND ";
                }
            }
        }

        query << ";";
        return query.str();
    }


};

int main() {
    SqlSelectQueryBuilder query_builder;
    query_builder.AddColumn("name")
        .AddColumn("phone");
    query_builder.AddFrom("students");
    query_builder.AddWhere("id", "42")
        .AddWhere("name", "John");

    std::string query = query_builder.BuildQuery();
    std::cout << query << std::endl;


    return 0;
}