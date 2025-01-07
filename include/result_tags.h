//
// Created by user1 on 30/11/2024.
//

#ifndef RESULT_TAGS_H
#define RESULT_TAGS_H

namespace m24
{

class ResultOkTag
{
};
class ResultErrTag
{
};

inline constexpr ResultOkTag OkTag;
inline constexpr ResultErrTag ErrTag;

} // namespace m24

#endif // RESULT_TAGS_H
