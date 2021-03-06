#ifndef client_h
#define client_h

#include "privilege.h"
#include "value.h"

#include <string>
#include <memory>

#include <paludis/util/private_implementation_pattern.hh>
#include <paludis/util/wrapped_forward_iterator.hh>

namespace eir
{
    class Bot;

    struct Channel;
    typedef std::shared_ptr<Channel> ChannelPtr;
    struct Client;
    typedef std::shared_ptr<Client> ClientPtr;
    struct Membership;
    typedef std::shared_ptr<Membership> MembershipPtr;

    struct Client : private paludis::PrivateImplementationPattern<Client>,
                    private paludis::InstantiationPolicy<Client, paludis::instantiation_method::NonCopyableTag>,
                    public std::enable_shared_from_this<Client>
    {
        const std::string& nick() const;
        const std::string& user() const;
        const std::string& host() const;
        const std::string& nuh() const;
        const std::string& account() const;

        void change_nick(std::string newnick);
        void set_account(std::string accountname);

        struct AttributeIteratorTag;
        typedef paludis::WrappedForwardIterator<AttributeIteratorTag,
                        std::pair<const std::string, Value> > AttributeIterator;

        AttributeIterator attr_begin();
        AttributeIterator attr_end();

        Value attr(const std::string &);
        void set_attr(const std::string &, const Value &);

        Client(Bot *, std::string, std::string, std::string);
        ~Client();

        MembershipPtr join_chan(ChannelPtr);
        void leave_chan(ChannelPtr);
        void leave_chan(MembershipPtr);

        struct ChannelIteratorTag;
        typedef paludis::WrappedForwardIterator<ChannelIteratorTag, const MembershipPtr> ChannelIterator;
        ChannelIterator begin_channels();
        ChannelIterator end_channels();
        MembershipPtr find_membership(std::string channel);
        ChannelIterator find_membership_it(std::string channel);
        const MembershipPtr find_membership(std::string channel) const;

        PrivilegeSet& privs();

        typedef std::shared_ptr<Client> ptr;
    };

    struct Channel : private paludis::PrivateImplementationPattern<Channel>,
                     private paludis::InstantiationPolicy<Channel, paludis::instantiation_method::NonCopyableTag>,
                     public std::enable_shared_from_this<Channel>
    {
        const std::string& name();

        struct MemberIteratorTag;
        typedef paludis::WrappedForwardIterator<MemberIteratorTag, const MembershipPtr> MemberIterator;
        MemberIterator begin_members();
        MemberIterator end_members();
        MembershipPtr find_member(std::string nick);
        MemberIterator find_member_it(std::string nick);

        bool add_member(MembershipPtr);
        bool remove_member(MembershipPtr);

        struct AttributeIteratorTag;
        typedef paludis::WrappedForwardIterator<AttributeIteratorTag,
                        std::pair<const std::string, Value> > AttributeIterator;

        AttributeIterator attr_begin();
        AttributeIterator attr_end();

        Value attr(const std::string &);
        void set_attr(const std::string &, const Value &);

        Channel(std::string);
        ~Channel();

        typedef std::shared_ptr<Channel> ptr;
    };

    struct Membership : private paludis::InstantiationPolicy<Membership, paludis::instantiation_method::NonCopyableTag>
    {
        Client::ptr client;
        Channel::ptr channel;

        std::string modes;

        bool has_mode(char m) { return modes.find(m) != std::string::npos; }

        typedef std::shared_ptr<Membership> ptr;

        Membership(Client::ptr cl, Channel::ptr ch)
            : client(cl), channel(ch)
        { }
    };
}

#endif
